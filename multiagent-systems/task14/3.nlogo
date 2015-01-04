globals 
 [
   run-duration
 ;  initial-num-marketers --Moved to slider
   
   unit-cost-of-business
   income-per-sale
 ;  money-size-ratio   --Moved to slider
   
   total-sales
 ]

turtles-own
 [
   weekly-sales
   weekly-cost
   bank-balance
 ]
 
to setup

   ca
   reset-ticks 
   
   set run-duration 200
;   set initial-num-marketers 200 --Moved to slider
   set total-sales 0
   
   set unit-cost-of-business 50
   set income-per-sale 2
;   set money-size-ratio 0.001  --Moved to slider
   
   ask patches [set pcolor green]
   
   crt initial-num-marketers
   
     [
       set size 1
       set shape "circle"
       setxy random-xcor random-ycor
       set bank-balance 0
     ]

  ; Open an output file for testing and analysis  TURNED OFF FOR EXPERIMENTS
  ; First, delete it instead of appending to it

;  if (file-exists? "TelemarketerSalesTest.csv") 
;    [carefully [file-delete "TelemarketerSalesTest.csv"] 
;      [print error-message]]

;  file-open "TelemarketerSalesTest.csv"
;  file-type "id,"
;  file-type "tick,"
;  file-type "Size,"
;  file-type "Bank bal,"
;  file-type "Potential customers,"
;  file-type "Max to call,"
;  file-type "Num called,"
;  file-print "Sales"
;  file-close
   
end
 
 
to go
 
  tick
  
  if ticks > run-duration [stop]
 
  ask patches [set pcolor green]
  
;  file-open "TelemarketerSalesTest.csv"
  ask-concurrent turtles [do-sales]
;  file-close
 
  ask turtles [do-accounting]
  
  output
  
end

 
to do-sales
 
   let max-calls size * 100
   let potential-customers patches in-radius (10 * size ^ 0.5)
   let customers-called potential-customers
   if max-calls < count potential-customers 
    [set customers-called n-of max-calls potential-customers]
   let buyers customers-called with [pcolor = green]
   
   set weekly-sales count buyers
   
   set total-sales total-sales + weekly-sales
   
   ask buyers [set pcolor brown]

  ; Test output   
 ; file-type who           file-type ","
 ; file-type ticks         file-type ","
 ; file-type size         file-type ","
 ; file-type bank-balance         file-type ","
 ; file-type count potential-customers         file-type ","
 ; file-type max-calls         file-type ","
 ; file-type count customers-called         file-type ","
 ; file-print weekly-sales
 
end
 

to do-accounting
 
   set weekly-cost unit-cost-of-business * size
   set bank-balance bank-balance - weekly-cost + (weekly-sales * income-per-sale)
   
   ifelse bank-balance > growth-param
    [
      set size (size + (bank-balance - growth-param) * money-size-ratio)
      set bank-balance growth-param
    ]
    [
      if bank-balance < 0 [die]
    ]
    
end
 

to output
 
   set-current-plot "Business Size Distribution"
   histogram [size] of turtles
 
   set-current-plot "Number of Businesses"
   plot count turtles
   
   set-current-plot "Total Sales"
   plot total-sales

end
@#$#@#$#@
GRAPHICS-WINDOW
454
10
765
342
100
100
1.5
1
10
1
1
1
0
0
0
1
-100
100
-100
100
1
1
1
ticks
30.0

BUTTON
28
30
91
63
NIL
setup
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

BUTTON
113
31
176
64
NIL
go
T
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

BUTTON
194
34
257
67
step
go
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

PLOT
24
109
224
259
Business Size Distribution
Size
NIL
0.0
200.0
0.0
10.0
true
false
"" ""
PENS
"default" 5.0 1 -16777216 true "" ""

PLOT
24
262
224
412
Number of Businesses
NIL
NIL
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -16777216 true "" ""

PLOT
238
108
438
258
Total Sales
NIL
NIL
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -16777216 true "" ""

SLIDER
29
70
245
103
initial-num-marketers
initial-num-marketers
50
500
200
50
1
NIL
HORIZONTAL

SLIDER
239
269
411
302
growth-param
growth-param
100
5000
1000
100
1
NIL
HORIZONTAL

SLIDER
239
310
417
343
money-size-ratio
money-size-ratio
0
0.005
0.001
.001
1
NIL
HORIZONTAL

@#$#@#$#@
# THE TELEMARKETER MODEL

## MODEL DESCRIPTION (ODD)

This model was produced by S. Railsback and V. Grimm for the book: _Agent-based and Individual-Based Modeling: A Practical Introduction_ (2012).

This version is the first, basic version of the model as described in Section 13.3.

## PURPOSE

The formerly remote land of Wasellya has developed very rapidly and experienced universal adoption of telephones. Naturally, the "invasion" of telephone technology is followed rapidly by an invasion of people tempted to start a telemarketing business. Is this a good business risk? How many telemarketers will stay in business, for how long? How does the average life span of a telemarketing company depend on how many are started?

The real purpose of this model is to illustrate several kinds of interaction: how to model them and what their effects are on system behavior. While the modeled system is imaginary and none too serious, it does represent a common ABM scenario: a system containing a number of agents that compete for a resource and grow or die as a result. 

## STATE VARIABLES AND SCALES

This model has two kinds of entities: potential customers (households with telephones), which we represent as patches, and telemarketers, which are turtles. The model is spatial: we represent the territory over which a telemarketer contacts potential customers, and the distance between telemarketers and potential customers, in arbitrary units equal to one patch size. World wrapping is turned off, so the space does have borders.

Potential customers have only one state variable in addition to their location. This is a boolean variable that represents whether they have been called by a telemarketer during the current time step. (In the NetLogo program you can use the patch's color for this variable: the patch is one color if it has not yet been called and a second color if it has been called.)

The telemarketers have a state variable "size" (we use NetLogo's built-in size variable) that represents the size of their company--how many staff and dialing machines, telephone lines, etc. they have. Telemarketers have a second state variable for their bank balance, their current financial reserves.

The model runs at a weekly time step. Simulations are 200 time steps long. There are 40,401 potential customers (the patches in a NetLogo world with the origin in the middle and maximum X and Y coordinates of 100).

## PROCESS OVERVIEW AND SCHEDULING

The model includes four actions executed in the following order each time step. 

First, the patches re-set their state variable for whether they have been called by a telemarketer this week to "no".

Second, the telemarketers all do their sales calls. Each marketer calls a number of potential customers; this number increases with telemarketer size. The potential customers are within a radius of the telemarketer that increases with the marketer's size. (In this early stage of Wasellya's development, the cost of phone calls increases steeply with their distance.) Each potential customer buys something if it has not been called previously in the time step, then sets its variable for whether it has been called to "yes". If that variable was already "yes", the potential customer hangs up on the telemarketer and buys nothing. (Wasellyans are not very sophisticated yet, but they have short tempers.)

The order in which the telemarketers do this second action is obviously important because the first marketer to call a customer makes a sale, whereas marketers who call later in the same week make no sale. We will explore this issue in Chapter 14 but here we use NetLogo's default scheduling, which is that the order in which telemarketers execute the action is randomly shuffled each time step.

Third, the telemarketers do their weekly accounting. Income is determined from the number of successful calls. Costs of business increase linearly with size. The bank balance state variable is updated by adding sales income and subtracting costs. If the bank balance is negative, the telemarketer goes out of business and is removed from the model. Finally, the telemarketer increases its size if its bank balance is high enough.

The fourth action is observer updates: outputs such as the number of telemarketers still in business, a histogram of their size, and the total number of sales, are reported.

## DESIGN CONCEPTS

**Emergence.** The primary model result we are interested in is patterns in the number of telemarketers in business over time as a simulation proceeds. (The size distribution of telemarketing companies--how  many big ones  and how many small ones there are, over time--is another potentially interesting result but we neglected it here.) These results emerge from how many telemarketers there are, how they compete with each other, how many customers there are and how they respond to calls, and how the telemarketers turn their income into growth.

**Adaptive behavior.** The telemarketers adapt to their sales success by deciding whether to grow in size, and how much to grow. However, this behavior is modeled very simply and rigidly: any bank balance above a minimum is automatically spent to increase in size. Potential customers have one simple behavior: when called, they decide whether to make a purchase by whether they have already been called during the time step. There are no adaptive tradeoff decisions.

**Prediction.** The telemarketer's adaptive behavior is based on an implicit prediction that if sales are higher than needed to maintain a minimum bank balance, then increasing in size will further increase income. (Note that this implicit prediction is different from--and perhaps not as smart as--predicting that growth will be beneficial as long as income is increasing. Is it smart to grow when income is high but decreasing?)

**Sensing.** The telemarketers can sense whether each potential customer has already bought something on the current time step and, hence, whether it makes a purchase or not.

**Interaction.** There are two kinds of interaction: between telemarketers and potential customers, and among the telemarketers. The telemarketers interact directly with potential customers by communicating to find out whether the customers will buy, and then by changing the customers' state to indicate that the customer has bought during the current time step. The telemarketers' interaction with each other is mediated by the resource they compete for: customers. When the territories of telemarketers overlap, customers of one telemarketer are no longer available as potential sales for other telemarketers.

**Stochasticity.**  There are three stochastic elements of the model. First, telemarketers are placed at random locations, uniformly distributed, when the model is initialized. Second, telemarketers select the customers they call randomly from all the potential customers (see the Telemarketer sales submodel, below). Third, the order in which telemarketers make their calls is randomly shuffled each time step, to avoid bias from the advantage that first callers have.

**Observation.** The primary outputs of interest can be observed via a plot of the number of telemarketers still in business. However, to understand how the system is working it is also useful to have a histogram of telemarketer sizes.  The number of telemarketers in business is output to a file each time step so that statistics on how long telemarketers stay in business (e.g., their median life span�the time at which half have failed) can be calculated at the end of a simulation.

## INITIALIZATION

The model is initialized with 200 telemarketers, each with size 1.0 and bank balance of 0.0, and a random location. (For display purposes, these are given random colors and drawn as circles.) The potential customers have their variable for whether they have been called during the week initialized to "no".

## SUBMODELS

**Telemarketer sales.** A telemarketer's potential customers are the patches within the marketer's territory, which is a circle around the marketer's location with radius equal to 10 times the square root of the telemarketer's size. The maximum number of calls a telemarketer can make each step is 100 times its size. (Therefore, unless the territory overlaps the space's edge, there are about 3.14 times more potential customers than calls.) If the number of potential customers is greater than the maximum number of calls, then the telemarketer randomly chooses this maximum number of potential customers to call. If instead the number of potential customers is less than the maximum number of calls (possible for marketers on the edge of the space), then all potential customers are called. The number of successful sales is the number of potential customers called who have not previously been called by a telemarketer on the same time step.

**Telemarketer accounting.** The weekly cost of business is equal to the telemarketer's size times 50. (We use arbitrary units for money.) The income from sales is 2 times the number of successful sales. The bank balance is updated by adding the income from sales and subtracting the cost of business. 

If the new bank balance is greater than 1000, then the telemarketer converts all but 1000 of this balance to increase in growth. For every 1 unit of bank balance above 1000, the telemarketer's size increases by 0.001. 

If the new bank balance is less than zero, the telemarketer goes out of business and is removed from the model.






This file provided as instructor materials for Railsback & Grimm (2012)
@#$#@#$#@
default
true
0
Polygon -7500403 true true 150 5 40 250 150 205 260 250

airplane
true
0
Polygon -7500403 true true 150 0 135 15 120 60 120 105 15 165 15 195 120 180 135 240 105 270 120 285 150 270 180 285 210 270 165 240 180 180 285 195 285 165 180 105 180 60 165 15

arrow
true
0
Polygon -7500403 true true 150 0 0 150 105 150 105 293 195 293 195 150 300 150

box
false
0
Polygon -7500403 true true 150 285 285 225 285 75 150 135
Polygon -7500403 true true 150 135 15 75 150 15 285 75
Polygon -7500403 true true 15 75 15 225 150 285 150 135
Line -16777216 false 150 285 150 135
Line -16777216 false 150 135 15 75
Line -16777216 false 150 135 285 75

bug
true
0
Circle -7500403 true true 96 182 108
Circle -7500403 true true 110 127 80
Circle -7500403 true true 110 75 80
Line -7500403 true 150 100 80 30
Line -7500403 true 150 100 220 30

butterfly
true
0
Polygon -7500403 true true 150 165 209 199 225 225 225 255 195 270 165 255 150 240
Polygon -7500403 true true 150 165 89 198 75 225 75 255 105 270 135 255 150 240
Polygon -7500403 true true 139 148 100 105 55 90 25 90 10 105 10 135 25 180 40 195 85 194 139 163
Polygon -7500403 true true 162 150 200 105 245 90 275 90 290 105 290 135 275 180 260 195 215 195 162 165
Polygon -16777216 true false 150 255 135 225 120 150 135 120 150 105 165 120 180 150 165 225
Circle -16777216 true false 135 90 30
Line -16777216 false 150 105 195 60
Line -16777216 false 150 105 105 60

car
false
0
Polygon -7500403 true true 300 180 279 164 261 144 240 135 226 132 213 106 203 84 185 63 159 50 135 50 75 60 0 150 0 165 0 225 300 225 300 180
Circle -16777216 true false 180 180 90
Circle -16777216 true false 30 180 90
Polygon -16777216 true false 162 80 132 78 134 135 209 135 194 105 189 96 180 89
Circle -7500403 true true 47 195 58
Circle -7500403 true true 195 195 58

circle
false
0
Circle -7500403 true true 0 0 300

circle 2
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240

cow
false
0
Polygon -7500403 true true 200 193 197 249 179 249 177 196 166 187 140 189 93 191 78 179 72 211 49 209 48 181 37 149 25 120 25 89 45 72 103 84 179 75 198 76 252 64 272 81 293 103 285 121 255 121 242 118 224 167
Polygon -7500403 true true 73 210 86 251 62 249 48 208
Polygon -7500403 true true 25 114 16 195 9 204 23 213 25 200 39 123

cylinder
false
0
Circle -7500403 true true 0 0 300

dot
false
0
Circle -7500403 true true 90 90 120

face happy
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 255 90 239 62 213 47 191 67 179 90 203 109 218 150 225 192 218 210 203 227 181 251 194 236 217 212 240

face neutral
false
0
Circle -7500403 true true 8 7 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Rectangle -16777216 true false 60 195 240 225

face sad
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 168 90 184 62 210 47 232 67 244 90 220 109 205 150 198 192 205 210 220 227 242 251 229 236 206 212 183

fish
false
0
Polygon -1 true false 44 131 21 87 15 86 0 120 15 150 0 180 13 214 20 212 45 166
Polygon -1 true false 135 195 119 235 95 218 76 210 46 204 60 165
Polygon -1 true false 75 45 83 77 71 103 86 114 166 78 135 60
Polygon -7500403 true true 30 136 151 77 226 81 280 119 292 146 292 160 287 170 270 195 195 210 151 212 30 166
Circle -16777216 true false 215 106 30

flag
false
0
Rectangle -7500403 true true 60 15 75 300
Polygon -7500403 true true 90 150 270 90 90 30
Line -7500403 true 75 135 90 135
Line -7500403 true 75 45 90 45

flower
false
0
Polygon -10899396 true false 135 120 165 165 180 210 180 240 150 300 165 300 195 240 195 195 165 135
Circle -7500403 true true 85 132 38
Circle -7500403 true true 130 147 38
Circle -7500403 true true 192 85 38
Circle -7500403 true true 85 40 38
Circle -7500403 true true 177 40 38
Circle -7500403 true true 177 132 38
Circle -7500403 true true 70 85 38
Circle -7500403 true true 130 25 38
Circle -7500403 true true 96 51 108
Circle -16777216 true false 113 68 74
Polygon -10899396 true false 189 233 219 188 249 173 279 188 234 218
Polygon -10899396 true false 180 255 150 210 105 210 75 240 135 240

house
false
0
Rectangle -7500403 true true 45 120 255 285
Rectangle -16777216 true false 120 210 180 285
Polygon -7500403 true true 15 120 150 15 285 120
Line -16777216 false 30 120 270 120

leaf
false
0
Polygon -7500403 true true 150 210 135 195 120 210 60 210 30 195 60 180 60 165 15 135 30 120 15 105 40 104 45 90 60 90 90 105 105 120 120 120 105 60 120 60 135 30 150 15 165 30 180 60 195 60 180 120 195 120 210 105 240 90 255 90 263 104 285 105 270 120 285 135 240 165 240 180 270 195 240 210 180 210 165 195
Polygon -7500403 true true 135 195 135 240 120 255 105 255 105 285 135 285 165 240 165 195

line
true
0
Line -7500403 true 150 0 150 300

line half
true
0
Line -7500403 true 150 0 150 150

pentagon
false
0
Polygon -7500403 true true 150 15 15 120 60 285 240 285 285 120

person
false
0
Circle -7500403 true true 110 5 80
Polygon -7500403 true true 105 90 120 195 90 285 105 300 135 300 150 225 165 300 195 300 210 285 180 195 195 90
Rectangle -7500403 true true 127 79 172 94
Polygon -7500403 true true 195 90 240 150 225 180 165 105
Polygon -7500403 true true 105 90 60 150 75 180 135 105

plant
false
0
Rectangle -7500403 true true 135 90 165 300
Polygon -7500403 true true 135 255 90 210 45 195 75 255 135 285
Polygon -7500403 true true 165 255 210 210 255 195 225 255 165 285
Polygon -7500403 true true 135 180 90 135 45 120 75 180 135 210
Polygon -7500403 true true 165 180 165 210 225 180 255 120 210 135
Polygon -7500403 true true 135 105 90 60 45 45 75 105 135 135
Polygon -7500403 true true 165 105 165 135 225 105 255 45 210 60
Polygon -7500403 true true 135 90 120 45 150 15 180 45 165 90

square
false
0
Rectangle -7500403 true true 30 30 270 270

square 2
false
0
Rectangle -7500403 true true 30 30 270 270
Rectangle -16777216 true false 60 60 240 240

star
false
0
Polygon -7500403 true true 151 1 185 108 298 108 207 175 242 282 151 216 59 282 94 175 3 108 116 108

target
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240
Circle -7500403 true true 60 60 180
Circle -16777216 true false 90 90 120
Circle -7500403 true true 120 120 60

tree
false
0
Circle -7500403 true true 118 3 94
Rectangle -6459832 true false 120 195 180 300
Circle -7500403 true true 65 21 108
Circle -7500403 true true 116 41 127
Circle -7500403 true true 45 90 120
Circle -7500403 true true 104 74 152

triangle
false
0
Polygon -7500403 true true 150 30 15 255 285 255

triangle 2
false
0
Polygon -7500403 true true 150 30 15 255 285 255
Polygon -16777216 true false 151 99 225 223 75 224

truck
false
0
Rectangle -7500403 true true 4 45 195 187
Polygon -7500403 true true 296 193 296 150 259 134 244 104 208 104 207 194
Rectangle -1 true false 195 60 195 105
Polygon -16777216 true false 238 112 252 141 219 141 218 112
Circle -16777216 true false 234 174 42
Rectangle -7500403 true true 181 185 214 194
Circle -16777216 true false 144 174 42
Circle -16777216 true false 24 174 42
Circle -7500403 false true 24 174 42
Circle -7500403 false true 144 174 42
Circle -7500403 false true 234 174 42

turtle
true
0
Polygon -10899396 true false 215 204 240 233 246 254 228 266 215 252 193 210
Polygon -10899396 true false 195 90 225 75 245 75 260 89 269 108 261 124 240 105 225 105 210 105
Polygon -10899396 true false 105 90 75 75 55 75 40 89 31 108 39 124 60 105 75 105 90 105
Polygon -10899396 true false 132 85 134 64 107 51 108 17 150 2 192 18 192 52 169 65 172 87
Polygon -10899396 true false 85 204 60 233 54 254 72 266 85 252 107 210
Polygon -7500403 true true 119 75 179 75 209 101 224 135 220 225 175 261 128 261 81 224 74 135 88 99

wheel
false
0
Circle -7500403 true true 3 3 294
Circle -16777216 true false 30 30 240
Line -7500403 true 150 285 150 15
Line -7500403 true 15 150 285 150
Circle -7500403 true true 120 120 60
Line -7500403 true 216 40 79 269
Line -7500403 true 40 84 269 221
Line -7500403 true 40 216 269 79
Line -7500403 true 84 40 221 269

x
false
0
Polygon -7500403 true true 270 75 225 30 30 225 75 270
Polygon -7500403 true true 30 75 75 30 270 225 225 270

@#$#@#$#@
NetLogo 5.1.0
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
<experiments>
  <experiment name="Num Marketers" repetitions="1" runMetricsEveryStep="true">
    <setup>setup</setup>
    <go>go</go>
    <metric>count turtles</metric>
    <metric>total-sales</metric>
    <steppedValueSet variable="initial-num-marketers" first="50" step="50" last="500"/>
  </experiment>
  <experiment name="NumMarketersXGrowthParameter" repetitions="1" runMetricsEveryStep="true">
    <setup>setup</setup>
    <go>go</go>
    <metric>count turtles</metric>
    <metric>total-sales</metric>
    <steppedValueSet variable="initial-num-marketers" first="50" step="50" last="500"/>
    <steppedValueSet variable="growth-param" first="1000" step="500" last="2000"/>
  </experiment>
  <experiment name="NumMarketersXMoneySizeRatio" repetitions="1" runMetricsEveryStep="true">
    <setup>setup</setup>
    <go>go</go>
    <metric>count turtles</metric>
    <metric>mean [size] of turtles</metric>
    <metric>mean [bank-balance] of turtles</metric>
    <steppedValueSet variable="initial-num-marketers" first="50" step="50" last="500"/>
    <enumeratedValueSet variable="money-size-ratio">
      <value value="5.0E-4"/>
      <value value="7.5E-4"/>
      <value value="0.001"/>
    </enumeratedValueSet>
  </experiment>
</experiments>
@#$#@#$#@
@#$#@#$#@
default
0.0
-0.2 0 0.0 1.0
0.0 1 1.0 0.0
0.2 0 0.0 1.0
link direction
true
0
Line -7500403 true 150 150 90 180
Line -7500403 true 150 150 210 180

@#$#@#$#@
0
@#$#@#$#@
