from collections import namedtuple n  

def updateController():
    Error = Desired - Actual
    Deriv = Actual - ActualPrev
    Integ+=Error
    
    if Integ > integMax:
        Integ = integMax
    if Integ < integMin:
        Integ = integMin
    
    Actuator = (p * Error) + (i * Integ) - (d * Deriv);
    
    if Actuator < actuatorMin:
        Actuator = actuatorMin
    if Actuator > actuatorMax:
        Actuator = actuatorMax

    Actuator += baseSpeed

def updateSystem():
    

print 'hello world!'

