#!/bin/sh
# run_capped.sh command [args...]
#
# Runs an automated (headless) game run and kills it when its memory grows
# past RR4_MEM_LIMIT_MB (default 6000). The limit is checked against the
# process's physical footprint, which on macOS includes GPU (Metal) memory
# that ps/RSS does not show: a GPU leak once took runs past 100 GB while RSS
# stayed at 200 MB. Use it for every scan, capture or test run, e.g.
#   BIN="tools/port/run_capped.sh build/ridge-racer-type-4-pc" ./scan.sh ...
# On Linux it falls back to RSS from /proc. RR4_MEM_LOG=<file> appends
# "<seconds> <MB>" for every check (the footprint over the run).
limit=${RR4_MEM_LIMIT_MB:-6000}
interval=${RR4_MEM_INTERVAL:-1}
log=${RR4_MEM_LOG:-}
start=$(date +%s)

"$@" &
pid=$!
trap 'kill -9 $pid 2>/dev/null' INT TERM HUP

footprint_mb() {
    if command -v footprint >/dev/null 2>&1; then
        footprint -p "$1" 2>/dev/null | awk '/phys_footprint:/ {
            v = $2; u = $3
            if (u ~ /GB/) v *= 1024; else if (u ~ /KB/) v /= 1024; else if (u ~ /^B/) v /= 1048576
            print int(v); exit }'
    elif [ -r "/proc/$1/status" ]; then
        awk '/VmRSS/ { print int($2 / 1024) }' "/proc/$1/status"
    fi
}

while kill -0 $pid 2>/dev/null; do
    sleep "$interval"
    mb=$(footprint_mb $pid)
    [ -n "$log" ] && [ -n "$mb" ] && echo "$(($(date +%s) - start)) $mb" >> "$log"
    if [ -n "$mb" ] && [ "$mb" -gt "$limit" ]; then
        echo "run_capped: killed pid $pid at ${mb} MB (limit ${limit} MB)" >&2
        kill -9 $pid 2>/dev/null
        wait $pid 2>/dev/null
        exit 137
    fi
done
wait $pid
