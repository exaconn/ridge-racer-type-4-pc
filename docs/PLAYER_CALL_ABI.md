# Player vehicle call interfaces

`UpdatePlayerCar` passes its `RaceVehicle *` to `IsCarFacingReverse`.
The original PAL call at 0x80022B88 uses the incoming `$a0`, which has
already been saved in `$s0`. The old C declaration returned `short` and
specified no arguments, while the actual definition returns `int` and reads
the vehicle. The caller now includes `game/ui/hudmap.h` and passes its
vehicle explicitly. Narrowing happens at the actual `half1EA` store.

`StepCarPhysics` likewise passes its vehicle to `UpdateCarSteering`.
The original call at 0x80022C9C retains the incoming `$a0`; saving it in
`$s0` occurs in the delay slot. `game/car/carsteer.h` now declares the
pointer argument for both this caller and `UpdateFinishingPlayerCar`.

`PlaceCarOnGrid` now passes its vehicle explicitly to `ResetVehicleShake`.
The reset takes the shared `RaceVehicle *` and clears fourteen named members
instead of addressing them through `s16 *` and `s8 *` casts. The store order
and widths remain those of the original function at 0x8002BEF8.

`test_player_reverse_call.py` compiles the four actual current bodies:
`UpdatePlayerCar`, `IsCarFacingReverse`, `StepCarPhysics` and
`ResetVehicleShake`. It compares
4096 native caller traces against original PAL instructions and another
4096 against the current linked ELF, including every service boundary,
complete vehicle records, signed player values, live global changes and
callee-saved register/stack guards. The physical simulation services are
instrumented at their boundaries; this does not test their algorithms.
All native pointers retain their full 64-bit values. An independent angular
interval oracle covers 131072 signed heading cases under UBSan.
Another 4096 whole-record reset cases execute both original and linked MIPS,
checking every store's address, width and order against native C and an
independent PAL-field oracle. Other vehicle fields and native owner guards
must remain unchanged. This tests the reset itself; the complete
`PlaceCarOnGrid` algorithm has separate reconstruction debt.

`SetVehicleShake` likewise takes the shared vehicle owner. Its x/y arguments
are complete signed words, as passed by `ResolveCarCollisions`; conversion to
sixteen bits occurs at the actual shake-field stores. `StartCollisionShake`
has an ANSI declaration with a vehicle and a signed-word impact kind. The
defining TU includes its own public header and the shared trigonometry/angle
interfaces. Local register-style names in the collision controller have been
replaced by the values' roles without changing their lifetimes or read order.

`test_vehicle_shake.py` compiles these two actual bodies and compares 16384
original/linked MIPS flows with native C under UBSan, whole-owner and stack
guards, full-word x/y values and impact kinds around the 2/3 boundary. It
checks the four setter stores independently and traces mutable trig/audio
service boundaries, including player/nonplayer identity and replay gates.
Trigonometry and audio algorithms themselves are outside this fixture.

The `RaceState` interval check uses unsigned subtraction to preserve PAL
wrapping for the full signed input domain. Existing unused stack storage in
`UpdatePlayerCar`, the extra ignored word passed to `ResetCarSlotBuffers`,
and other local declarations remain reconstruction debt. No new register
bindings, empty assembly constraints or padding were introduced.

During validation, the model-bank offset helper needed its signed 32-bit
offset decoding restored before addition to the complete native bank
address. The previous simplification changed `RegisterModelBank` to
relocated instruction Levenshtein distance 3 and failed its existing native
lifecycle test. The repaired function has distance 0. CD sector-to-byte
conversion also shifts an unsigned word before publishing the signed word,
preserving MIPS wrapping without a negative signed shift in C.

`SsVabClose` has one public signed-halfword declaration in
`psyq/libsnd/vmcfg.h`, included by the audio-bank interface and fade callers.
Its definition checks the unsigned low halfword before indexing a bank.
`InitDemoFadeContext` keeps the captured handle as `s16`, matching its actual
audio-owner field. This preserves the original call without adding caller
sign-extension instructions. Its existing bank-release test now selects the
actual defining TU rather than a generated view and calls its signed-halfword
interface; the full encoded-halfword domain, models and assertions are unchanged.
The release body now indexes `_svm_vab_used` and `_svm_vab_start` directly;
the two register bindings and artificial pointers to 0x80110000 are removed.
Its native test compiles that body without register or address rewriting,
binding only the real global arrays and count to the existing fixture owner.

Some casts removed concurrently were necessary: signed 32-bit ordering biases
must be sign-extended before adding a full-width native ordering pointer,
and the spark path must narrow a shifted word to signed 32 bits before
shifting it right in a native-width temporary. The unsigned frame sum in
flash-quads also prevents signed overflow. Existing native tests exposed
these regressions; their inputs, models and assertions were preserved.

`DrawRaceOverlaySprites` remains a separate interface problem. Converting
its public unspecified-arguments declaration to a one-argument prototype
breaks overlay 659: its original PAL call consumes the `$a0` left by
`UpdatePauseMenu`, with no new argument setup. The incomplete header change
was rejected; the ANSI definition is retained, and the old declaration
remains explicit debt. No arbitrary paused argument was substituted.


## Decay handler and stage table

`DecayCarOffset` now takes the common `RaceVehicle *` and updates its actual
position, shake and control fields. Its old private `W10_5A` prefix hardcoded
the PAL control offset. On a 64-bit host that location belongs to
`control.playerFlags24`; the actual `control.pathDeltaX30` moves with the real
pointer-bearing control owner. The recovered owner avoids this incorrect view.
Position subtraction explicitly wraps signed-word encodings, matching MIPS
SUBU for the complete input range without signed-overflow undefined behavior.

`game/car/cartimer.h` declares the actual handler. `game/race/racemai2.h` owns
`CarUpdateStage`, a function pointer taking the same vehicle, and the eleven
entry `CarUpdateStages` table. Original EXE entries and all eleven current C
definitions agree with that contract. The first seven and final four stages
are dispatched by the unchanged `StepRaceFrame` loops.

`test_vehicle_decay.py` executes the real decay body through that public table
type, with three complete native cars and two actual shared record/handling
pointees. Its 8192 cases run against original and linked MIPS (16384 flows),
including signed timer/offset boundaries and wrapping both position axes.
All serialized fields, pointees, unselected cars, native alignment/guards and
saved-register/stack guards are checked. A targeted case preserves player
flags while clearing the correct control member. The fixture tests this
handler and its table ABI, not the other ten stage algorithms or the complete
race-frame algorithm. Native code removes only the pre-existing physical
register name; the existing identity-copy asm remains part of the actual body.

The pre-existing register binding and empty-copy barrier are retained debt.
Removing them or extracting the component arithmetic into an inline helper did
not preserve instruction distance zero. No new constraints or compiler options
were introduced; the existing per-unit compiler selection is unchanged.


## Collision geometry owner

`ScratchpadCollision` and its packed `CollisionPoint` are now owned by
`rr/collision_work.h`. The controller and its three geometry helpers share
those types and public declarations in `game/race/racemai2.h`. The nine points
of each footprint are the center, four corners and four edge midpoints.
`ComputeQuadMidpoints` uses these named slots and signed x/y members rather
than indexing a cast halfword pointer. `PointInQuadPtr` consumes an actual
array of packed points. Both point tests express the four nonnegative clip
results directly with short-circuit `&&`, preserving live load order without
repeated zero assignments or a complemented-word sign-bit expression.
Local names in the race-frame entry now describe their roles. `StepRaceFrame` has an ANSI
entry declaration; its operations and all local lifetimes are preserved.

`test_collision_geometry.py` compiles all three actual current helper bodies
without body rewriting. It binds scratch work to a complete native object at
a full-width address, and compares 24576 original/linked MIPS flows with native
C under UBSan. Complete 1KB scratch snapshots, both car footprints, an extra
corner array, native/ISA guards, store order and saved-register/stack guards
are checked. Boundary, negative, degenerate and random coordinate cases cover
every early exit and both final outcomes. Mutable NormalClip service calls
check that later corners are read live; that service uses signed-word GTE
area semantics, but this fixture does not emulate the GTE or test the complete
ResolveCarCollisions algorithm. Midpoints also have an independent Python
whole-state oracle using floor division for negative coordinates.

The controller's pre-existing `*(s32 *)(points + index)` packed-word read
remains explicit reconstruction debt. Replacing it with the union member,
a copied point or a typed pointer changed the original address calculation;
those candidates were rejected. A separate named-quad view likewise failed
for the midpoint helper. No new register pins, empty asm, padding, dummy
operations or toolchain options were introduced. The accepted direct typed
array form has relocated instruction distance zero for all nine TU functions.


## Race-frame trig and impact interfaces

`StepRaceFrame` now includes `rr/trig.h` and calls the actual one-word `rcos`
interface. The two extra ignored vehicle-pointer arguments and local
unspecified-arguments declaration are removed. No replacement argument,
assembly constraint or ABI shim was added. Course grouping explicitly
converts the course index to its signed halfword before halving, replacing
the decompiled left/right shift sequence. The controller's current boolean
collision helpers are preserved.

`SetCarImpactState` likewise has one public declaration taking a signed-word
state and the actual vehicle pointer. Its definition uses ANSI parameters,
role-based locals and the shared `AngleDiffAbs` header. Its existing absolute
value register binding remains explicit reconstruction debt; this change
does not remove or add constraints or alter the impact algorithm.

`test_race_frame_calls.py` compiles the actual current `StepRaceFrame` body
unchanged and compares 4096 native flows against original MIPS and another
4096 against current linked instructions. It reaches both cosine call sites,
all eleven stage boundaries, every game-mode branch, signed impact kinds and
both active/inactive cars. It uses three full native vehicle owners, shared
record/handling pointees, aliased live car tables, actual function pointers
and the complete scratchpad. Service mutations change counts, car pointers,
course selection and stage pointers between calls; whole-state snapshots
and call traces check cached values and live reads. Original and linked mode
jump tables are also checked. Native/ISA guards, saved registers, stack and
controller store widths/locations must remain intact.

The fixture instruments input, player/AI physics, impact/shake, angular/trig,
projection and stage algorithms at their real interfaces. It tests frame
control and impact/contact arithmetic, not the algorithms behind those
boundaries. All eight racing course indices and the four-byte contact table
are exercised. The original2048 six-index cases are retained exactly and an
additional2048 cover the eight-index domain. Positive speed denominators and
bounded drive products keep the tested arithmetic in its defined C domain.
Signed speed/impact boundaries remain in the cases.
No pre-existing fixture seeds, inputs, aliases, models or assertions changed.


## Actual impact and shake controllers

`game/car/carstate.h` now owns both `SetCarImpactState` and `UpdateCarShake`
interfaces over the shared vehicle owner. The latter's waveform/amplitude
locals describe their roles rather than MIPS registers. Its quarter-pitch
update uses ordinary signed division and compound angle assignment. The
manual negative-rounding adjustment and unsigned-halfword angle-add casts
are removed; narrowing still happens at the real signed-halfword stores.
No physical register pin or empty assembly constraint was added.

`test_impact_controllers.py` compiles both actual production bodies, removing
only the setter's existing physical MIPS register annotation for the host.
It compares each function and their chained flow against original and linked
MIPS: 4096 cases for each operation and engine, 24576 flows altogether. Inputs
include complete three-car native owners, shared full-width record/handling
pointees, full-word states, signed-halfword boundaries, every byte timer value,
unknown state encodings, angle wrapping and replay gates. Angular, sine,
haptic and audio boundaries can mutate live vehicle/global data; complete
call traces and whole-owner/pointee snapshots check reads around those calls.
ISA stores are restricted to the selected real field locations and widths;
stack/saved-register guards, native owner/alignment guards and ISA pointee
boundary guards are checked.
256 nonmutating cases per engine also use an independent whole-state setter
oracle. Timer expiry and contact-timer underflow paths must be reached.

The fixture tests these controllers and their combined transition, not the
angular/sine/audio/device algorithms behind the instrumented boundaries.
`SetCarImpactState` retains its existing absolute-value register pin. Plain
C field/conditional forms changed instruction distance; standard `abs` with
an explicitly narrowed input came closer but still had distance4. Those
candidates remain unadopted, with no compensating operations or compiler
changes. Splitting all transient shake values into new locals or replacing
only the standalone manual division did not preserve the match either; the
accepted compound angle expression does, for the complete TU.
No pre-existing test inputs, seeds, models or assertions were changed.


## Shared model submission and intro scratch owner

`rr/rro_model.h` owns one `SubmitModel` declaration: an actual
`RroSceneScratchPrefix *` followed by three signed words. Its handwritten
backend consumes this prefix and the complete scratch arena following it;
this change does not reimplement that backend. Six resident caller TUs now
include the common declaration. Their old unspecified/int-address/void-pointer
local declarations are removed; fixed console scratch addresses are explicitly
converted to the scene-pointer type. This preserves full native addresses
when a caller binds scratch storage to a native object.

`RroModelSetup` describes the existing intro angles/rotation/light setup as
another view of `RroScratchWork`. It fits in the existing union and changes
neither PAL nor native owner layout. `DrawRotatingTitleModel` uses the real
owner for work, view matrix and draw counter rather than PAL offsets. On a
64-bit host the existing owner has its matrix at48, work at160 and counter
at152; the old offsets40/124/120 addressed the wrong native members. Its
public prototype now takes the real two words, and five ignored pointer
arguments were removed from the scene controller's calls. Both complete
caller TUs retain instruction distance zero.

`test_model_submission_caller.py` compiles the actual current title-rendering
caller without body rewriting. Its 2048 cases run against original and linked
MIPS (4096 flows), including all eight object indices, signed yaw/flag limits,
full-width global owner and submission pointers, model-bank prefix changes,
live intro-owner replacement and format changes. Complete canonical native
scratch/intro owners, typed pointees and GTE matrix transports are compared
at every boundary and finally; native owner/union guards, ISA store bounds,
stack and saved registers are checked. Matrix construction, bank selection,
object transformation and submission algorithms are instrumented boundaries.
This is native coverage of the caller, not the complete scene state machine
or the handwritten model submission backend.

A compiler layout probe confirms that the added union member moves no
existing native owner field. The existing animated track-dispatch fixture
changes only its SubmitModel stub's first parameter to the shared scene type;
an exact reverse adapter proves all old cases/models/assertions unchanged.
Other pre-existing fixture files are preserved.


## Transformed title model inputs

`DrawTransformedTitleModel` now takes an actual `const Vec3s8 *` angle record
and `const VectorWords16 *` position instead of two integer addresses. The
three scene-controller calls pass the matching typed eight-byte angle and
sixteen-byte position objects. The local matrix is a `MatrixWords32`; work,
light, view-matrix, transform and counter accesses use the real scratch owner.
Its obsolete local forward declaration and fixed PAL offset casts are removed.
The complete caller and callee TUs retain relocated instruction distance zero.

`test_transformed_title_model.py` adds 2048 original and 2048 linked flows
using the established intro transport/service models. It compiles the actual
body unchanged, with real typed angle/vector union owners at full native
addresses, independent input rows, both aliased and distinct input pointers,
full-word model selection boundaries, complete owner/pointee snapshots,
mutable global intro-owner and bank-format services, GTE transport events,
exact ISA store bounds and native/ISA guards. The previous rotating-title
fixture is unchanged and still checks its original2048 cases per engine.
The new fixture checks this controller's pointer/data flow and selection,
not matrix construction, transform or handwritten submission algorithms.

## Projected textured quads

`EmitProjectedQuadsFT4` takes the shared `RroTexturedQuadPrefix` and a complete
`RroEffectWork`. Its three call sites use the common declaration. The command
prefix now shares `RroQuadIndices`, preserving its 28-byte size and field offsets
while exposing both packed words and individual indices. Shadow and flash
consumers use that same view. Their two existing native fixture adapters change
only the corresponding member paths; seeds, cases and reference operations stay
unchanged. The unused local `Quad1C` duplicate is removed.

The emitter uses actual native scratch, projected-vertex, ordering and GPU packet
owners, including the shifted native pointer fields. Depth sums wrap as 32-bit
words; signed ordering bias uses pointer arithmetic. Only DMA links are encoded
through the registered GPU address interface. Vertex reads index the complete
63-entry pipeline from slot 3, so signed upper indices can also address its
three leading entries within the same array. The numeric vertex addresses and
command-cursor empty asm are removed. Existing volatile scratch reads remain
reconstruction debt; removing that qualifier currently gives instruction
distance 3 and is not adopted.

`test_projected_textured_quads.py` compiles the actual body without rewriting.
It checks 8468 flows against original instructions and 8468 against the linked
function, with an independent wire oracle, whole native owners and guards,
negative/zero counts from the signed command-header half, signed upper indices
including the three leading pipeline slots, wrapping depth sums, every valid
shift, every signed bias and all ten packet/head overlap positions. GTE transfer
order and scripted NCLIP results are checked. Store-time mutations verify the
captured geometry/shift/winding/cursor and live UV/bias/order/color fields.
This covers the emitter; numerical NCLIP, GTE flags/timing and the complete
`DrawWheelTrailEffects` caller remain outside this fixture.

## Projected trail panels

The legacy `EmitProjectedLitTrisGT3` name denotes three untextured G4 panels
followed by an E1 draw-mode command. `RroTrailPanelCommand` describes the three
outer indices, common center and signed ordering bias in the actual 12-byte
record. `GpuPolyG4` shares the existing byte/word packet views and DMA tag;
the trailing command uses the existing `GpuCommandPacket`. All three callers
use the public command/work pointer prototype. The local `Tri0C` duplicate is
removed. This emitter requires no changes to existing native fixtures.

The emitter now indexes two real projected-point arrays, extracts signed X/Y
halves, accesses the actual native workspace fields and preserves complete
pointer widths. Its depth interval test uses unsigned subtraction so an
`INT32_MIN` depth follows MIPS word semantics without signed overflow. It has
no register bindings, empty asm, numerical pointer arithmetic or fake volatile.
The typed cursor traverses three 36-byte panels and the trailing 8-byte E1
packet, preserving partial XY staging, write order and all four DMA links.

`test_projected_trail_panels.py` compiles that body unchanged and checks 4410
original plus 4410 linked flows against an independent wire oracle under
UBSan/bounds checks. Cases include both aliased/distinct projected slices,
signed header counts, extreme signed depths, all valid shifts, every signed
bias, all 29 packet/head overlap positions, clipping boundaries, complete
native owners and guards, exact stores and saved-register/stack guards.
GTE-store mutations distinguish captured slice roots, colors, winding, shift
and cursor from live indices, coordinates, ordering roots and stored depth.
Transfer order and scripted NCLIP results are checked; numerical GTE accuracy,
flags/timing and the full `DrawWheelTrailEffects` caller are outside this test.


## Projected glow points

`EmitProjectedLitQuadsGT4` retains its legacy name but emits eight untextured
G3 triangles and a trailing E1 command for each visible point (232 bytes).
Its input is an eight-byte `RroGlowPointCommand`, with the signed ordering bias
in the high byte of the position padding word. `RroGlowProjection` owns the
adjusted GTE translation, packed screen center and actual ordering-table cell
pointer. Twenty-four records fit the PAL scratch workspace. Native records
retain full-width pointers, so the native union/work/scratch owners grow; their
PAL layout assertions remain unchanged. The identity rotation has a real
matrix view in that same shared workspace. The nine-entry ring is declared
with its actual packed vertex type, including the repeated closing vertex.

The emitter uses typed records, existing G3/E1 packet types and the shared
GPU-address interface. Numeric pointer arithmetic/truncation, register-style
local names, manual signed-division expansion and all three gotos are removed.
Six of its seven pre-existing physical register bindings are retired. The
remaining binding for the previous projected ring vertex is explicit debt:
removing it still changes instruction distance and is not accepted. No new
pins, empty assembly, fake volatile, dummy operations or compiler options were
introduced. The caller now consumes the shared typed declaration, but its
complete `DrawWheelTrailEffects` ownership/algorithm remains separate debt.

`test_projected_glow_points.py` compares the actual current body with an
independent wire oracle and original/linked MIPS in 2394 cases per engine.
Only the single existing physical register annotation is removed for native
compilation; operations and lifetimes are unchanged. Native objects keep their
64-bit addresses and are checked in full under UBSan/bounds instrumentation.
Coverage includes all 256 signed biases, 32 shifts and 58 word positions for
ordering-head/packet overlap, signed header counts, the 24-point capacity,
extreme facing/depth values, exact GTE transfers, saved-register/stack guards,
and mutable input bias, ring, roots and colors. Offsets/shift and staged heads
are distinguished from later live values. RTPS outputs are scripted: GTE
numerical accuracy, flags, pipeline timing and the complete caller are outside
this fixture. Existing test inputs, models and assertions are unchanged.


## Shared RotMatrix contract and wheel geometry

All fourteen `RotMatrix` calls now consume the one declaration in
`psyq/libgte/rotmat.h`: signed halfword angles in a real `Vec3sFields8`,
a complete `MatrixWords32` destination, and that destination pointer as the
return value. The original hand-written assembly reads x/y/z before its first
store, writes nine rotation halfwords and preserves padding/translation. The
input and output may overlap. The assembly and its hasm classification are
unchanged. Eight resident TUs and two overlay callers now share this contract;
local unspecified-argument, void/int-return and halfword-pointer declarations
are removed. Actual stack matrices/vectors in wheel, drive, sound and track
object callers have their real types; no casts disguise a mismatched owner.

The wheel helper also exposed an out-of-bounds prefetch in its old C owner:
after projecting the fourth corner it reads the following pair of collision
heading limits. `VehicleFootprint20` owns the four corners and two limits, with
an actual five-pair view for that prefetch. All writers/readers use the same
shared owner; its size, alignment and vehicle offset stay unchanged on PAL and
native hosts. Two legacy fixture views receive only the reversible field-name
substitutions. The former six-byte local vector is an actual eight-byte GTE
input object. Its ignored padding is not artificially initialized. The IR
store uses the existing memory-declaring hardware intrinsic, without adding
instructions or compiler steering constraints.

`test_rotation_contract.py` checks the actual original/linked assembly against
a native implementation of the public contract in 200704 cases per engine:
every signed-halfword value on each axis, 4096 mixed triples and five actual
union owner layouts (four overlapping, one disjoint). Complete output owners,
preserved input/translation/padding/guards and the returned pointer are checked
under UBSan/bounds instrumentation. This native implementation is a fixture
witness, not a replacement for the production hand-written routine.

`test_wheel_projection.py` executes the actual current `ComputeWheelOffsets`
body without rewriting. Its 4096 original and 4096 linked flows execute the
real rotation assembly and compare exact defined GTE transfers, two complete
native vehicle owners, live corner/limit and wheel-height mutations, the
prefetch-before-store order, saved-register/stack guards and native bounds.
Projection outputs are scripted; GTE math, flags and hardware timing are
outside this fixture. VZ0/R33 ignore their unused upper halves as the hardware
does; all three vector coordinates and nine matrix elements remain checked.
Six existing fixtures receive only type/equivalent-view adapters for this ABI;
their cases, seeds, service mutations, models and assertions are preserved.
