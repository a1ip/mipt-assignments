globals
 [
   ; Model run parameters
   years-simulated
   num-investors
   
   ; Profit landscape parameters
   patch-min-profit
   patch-max-profit
   patch-min-risk
   patch-max-risk
   
   ; Investor parameters
   decision-time-horizon
  ]
  
patches-own 
 [
  profit
  annual-risk
 ]
 
turtles-own
 [
  wealth
  current-utility
 ]
  
to setup

  clear-all
  reset-ticks

 ; Initialize global variables
  set years-simulated    25

  set num-investors 100

  set patch-min-profit    1000
  set patch-max-profit    10000
  set patch-min-risk  0.01
  set patch-max-risk  0.1

  set decision-time-horizon 5

 ; Initialize patch profit and risk characteristics
  ask patches
    [
     ; Set the profitability
     set profit patch-min-profit + random-float (patch-max-profit - patch-min-profit)
    
     ; Set the risk 
     set annual-risk patch-min-risk + random-float (patch-max-risk - patch-min-risk)

     ; Color patches by profit: greener is richer
     set pcolor scale-color green profit patch-max-profit patch-min-profit
     
     ; Label patches by risk
     set plabel precision annual-risk 2
    ]
    
 ; Create the investors
 crt num-investors
  [
   move-to one-of patches with [not any? turtles-here]
   set wealth 0.0
  ]
  
 ; Open an output file for testing and analysis
 ; THIS WILL NOT WORK if you use BehaviorSpace running on more than one processor!
 ; First, delete it instead of appending to it
   if (file-exists? "TestOutput-Investors.csv") [carefully [file-delete "TestOutput-Investors.csv"] [print error-message]]
   file-open "TestOutput-Investors.csv"
      file-type "id,"   ;Fascinating! If you use "ID" instead of "id", Excel will not open the file.
      file-type "tick,"
      file-type "wealth,"
      file-type "profit,"
      file-type "risk,"
      file-type "utility,"
      file-print "wealth-increase-rate"
   file-close
   
 ; Finally, write output for initial state of model
 output

end

to go
  
  tick

  if ticks > years-simulated [stop]
      
  ask turtles [reposition]
  
  ask turtles [do-accounting]
  
  output
  
end

to reposition

  ; Move if the expected wealth increase rate is below the threshold
  ; parameter defined on the Interface slider.
  ; Potential destinations do NOT include the current patch.
  
  if current-utility < wealth-increase-threshold
  [
    let potential-destinations neighbors with [not any? turtles-here]
    if any? potential-destinations [move-to one-of potential-destinations]
  ]
 
end

to do-accounting

  ; First, add this year's profits
  set wealth (wealth + profit)
  
  ; Now see if the investment failed
  if random-float 1.0 < annual-risk [set wealth 0]
  
  ; Update the utility of the investor
  set current-utility utility-here

end

to-report utility-here    ; A turtle procedure that evalutes its patch
 ; This version uses the satisficing threshold approach

 ; First calculate expected wealth over the time horizon
 if wealth = 0 [set wealth 1.0]   ; Prevent division by zero after failure and on tick 1
 let utility wealth + (profit * decision-time-horizon)
 
 ; Then factor in risk of failure over time horizon
 set utility utility * ((1 - annual-risk) ^ decision-time-horizon)
 
 ; Now report utility as expected annual rate of wealth increase,
 ; the expected change wealth over the time horizon divided by
 ; the time horizon length
 let wealth-increase-rate (utility - wealth) / wealth
 set wealth-increase-rate (wealth-increase-rate / decision-time-horizon)
 
 report wealth-increase-rate

end


to output

    ; Note that utility histogram needs to be re-scaled; utility now varies between zero and one
    set-current-plot "UtilityHistogram"
    histogram [current-utility] of turtles
    
    set-current-plot "Mean Wealth"
    plot mean [wealth] of turtles

    set-current-plot "Mean Profit"
    plot mean [profit] of turtles

    set-current-plot "Mean Risk"
    plot mean [annual-risk] of turtles
    
    ; Write turtle states to output file
    file-open "TestOutput-Investors.csv"
    ask turtles
     [ 
      file-type who          file-type "," 
      file-type ticks          file-type "," 
      file-type wealth          file-type "," 
      file-type profit          file-type "," 
      file-type annual-risk          file-type "," 
      file-type current-utility          file-type "," 
      file-print utility-here
     ]
   file-close

end
@#$#@#$#@
GRAPHICS-WINDOW
446
10
836
421
9
9
20.0
1
8
1
1
1
0
0
0
1
-9
9
-9
9
1
1
1
ticks
30.0

BUTTON
11
10
74
43
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
89
10
152
43
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
165
11
228
44
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
11
91
211
241
UtilityHistogram
NIL
NIL
0.0
0.5
0.0
10.0
false
false
"" ""
PENS
"default" 0.01 1 -16777216 true "" ""

PLOT
242
10
442
160
Mean Profit
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
243
163
443
313
Mean Risk
NIL
NIL
0.0
10.0
0.0
0.1
true
false
"" ""
PENS
"default" 1.0 0 -16777216 true "" ""

PLOT
10
248
210
398
Mean Wealth
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
11
50
230
83
wealth-increase-threshold
wealth-increase-threshold
0
1
0.05
.01
1
NIL
HORIZONTAL

@#$#@#$#@
## THE BUSINESS INVESTOR MODEL


# MODEL DESCRIPTION (ODD)

This model was produced by S. Railsback and V. Grimm for Chapter 10 of the book Agent-Based and Individual-Based Modeling (2012).

This version of the model is described in Section 11.4. It replaces the "reposition" submodel with a satisficing trait. This model description has not been updated with this change.

# PURPOSE

The primary purpose of this model is to explore effects of "sensing"--what information agents have and how they obtain it--on emergent outcomes of a model in which agents make adaptive decisions using sensed information. The model uses investment decisions as an example, but is not intended to represent any real investment approach or business sector.

This model could be thought of as approximately representing people who buy and operate local businesses: it assumes investors are familiar with investment opportunities within a limited range of their own experience, and that there is no cost of entering or switching investments (e.g., as if capital to buy a business is borrowed and the repayment is included in the annual profit calculation).

# STATE VARIABLES AND SCALES

The entities in this model are investor agents (turtles) and business alternatives (patches) that vary in profit and risk. The investors have state variables for their location in the space and for their current wealth (_W_, in money units).

The landscape is a grid of business patches, which each have two static variables: the annual profit (_P_; in absolute money units such as dollars) an investor would obtain there, and the annual risk of the investment there failing and losing all its value (_F_; probability per year). This landscape is 19 by 19 patches in size with no wrapping at its edges. 

The model time step is one year, and simulations run for 25 years.

# PROCESS OVERVIEW AND SCHEDULING

The model includes the following actions that are executed in this order each time step.

**Investment repositioning:** The investors decide whether any similar business (adjacent patch) offers a better tradeoff of profit and risk; if so, they "reposition" and transfer their investment to that patch, by moving there. Only one investor can occupy a patch at a time. The agents execute this action in randomized order.

**Accounting:** The investors update their wealth state variable. _W_ is set equal to the previous wealth plus the profit of the agent’s current patch. However, unexpected failure of the business is also included in the accounting action. This event is a stochastic function of _F_ at the investor's patch. If a uniform random number between zero and one is less than _F_, then the business fails: the investor's wealth becomes zero, but the investor stays in the model and nothing else about it changes.

**Output:** The world display, plots, and an output file are updated. 

# DESIGN CONCEPTS

Basic principles: The basic topic of this model is how agents make decisions involving tradeoffs between several objectives—here, increasing profit and decreasing risk.

Emergence: The model's primary output is the mean investor value, over time. Important secondary outputs are the mean profit and risk chosen by investors over time, and the number of investors who have suffered a failure. These outputs emerge from how individual investors make their tradeoff decisions, but also from the "business climate": the ranges of _P_ and _F_ values among patches and the number of investors competing for locations on the landscape.

Adaptive behavior: The adaptive behavior of investor agents is their decision of which neighboring patch to move to (or whether to stay put), considering the profit and risk of these alternatives. Each time step, investors can reposition themselves to occupy any unoccupied one of the eight adjacent patches in the business landscape, or retain their current position. In this version of the model, investors use a simplified microeconomic analysis to make their decision, moving to the patch providing highest value of an objective function. 

Objective: (In economics, the term "utility" is used for the objective that agents seek.) Investors rate alternative investment positions by a utility measure that represents their expected future investment value at the end of a time horizon (_T_, a number of future years; we use 5). This expected future wealth is a function of their current investment value, the profit offered by the patch, and the risk of failure at the patch:

  _U_ = (_W_ + _T_ _P_) (1-_F_)<sup>_T_</sup>

where _U_ is expected utility for the patch, _W_ is the investor's current value, and _P_ and _F_ are defined above. The term (_W_ + _T_ _P_) estimates the investment value at the end of the time horizon. The term (1-_F_)<sup>_T_</sup> is the probability of surviving failure over the time horizon; it reduces utility more as failure risk increases. (Economists might expect to use a utility measure such as present value that includes a discount rate to reduce the value of future profit. We ignore discounting to keep this model simple.) 

Prediction: The fitness measure includes an explicit forecast of utility over a time horizon that uses the assumption that _P_ and _F_ do not change over time. This assumption is accurate here because the patches' _P_ and _F_ values are static. 

Sensing: The investor agents are assumed to know the profit and risk at their own patch and the adjacent neighbor patches, without error. 

Interaction: The investors interact with each other only indirectly via competition for patches: an investor cannot reposition itself into a patch that is already occupied by another investor. Investors execute their repositioning action in randomized order, so there is no hierarchy in this competition: investors with higher investment value have no advantage over others in competing for locations. 

Stochasticity: The initial state of the model is stochastic: the values of _P_ and _F_ of each patch, and initial investor locations, are set randomly. Stochasticity is thus used to simulate an investment environment where alternatives are highly variable and risk is not correlated with profit. Whether each investor fails each year is also stochastic, a simple way to represent risk. The investor reposition action uses stochasticity only in the very unlikely event that more than one potential destination patch offers the same highest utility; when there is such a tie the agent randomly chooses one of the tied patches to move to. 

Observation: The World display shows the location of each agent on the investment landscape. Graphs show the mean risk and mean profit of patches occupied by investors, and mean investor wealth over time. An output file reports the state of each investor at each time step.

Learning and collectives are not represented. 

# INITIALIZATION

Four model parameters are used to initialize the investment landscape. These define the minimum and maximum values of _P_ (10,000 and 100,000) and _F_ (0.1 and 0.01). The values of _P_ and _F_ for each patch are drawn randomly from uniform real number distributions with these minimum and maximum values. 

One hundred investor agents are initialized and put in random patches, but investors cannot be placed in a patch already occupied by another investor. Their wealth state variable _W_ is initialized to zero. 

# INPUT DATA

No time-series inputs are used.

# SUBMODELS

**Investor repositioning:** An agent identifies all the businesses that it could invest in: the neighboring (eight, or fewer if on the edge of the space) patches that are unoccupied, plus its current patch. The agent then determines which of these alternatives provides the highest value of the utility function, and moves (or stays) there.

**Accounting:** This action is fully described above ("Process overview and scheduling").




This file provided as instructor materials for Railsback & Grimm 2012.
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
NetLogo 5.0RC4
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
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

-- 
Вы получили это сообщение, поскольку подписаны на группу multiagent-2014.

Чтобы отменить подписку на эту группу и больше не получать от нее сообщения, отправьте письмо на электронный адрес multiagent-2014+unsubscribe@googlegroups.com.
Чтобы добавлять сообщения в эту группу, отправьте письмо по адресу multiagent-2014@googlegroups.com.
Просмотреть это обсуждение в Сети можно по адресу https://groups.google.com/d/msgid/multiagent-2014/546DF6A3.6%40gmail.com.
Настройки подписки и доставки писем: https://groups.google.com/d/optout.
