import math

def calc_smallest_angle_difference(desired, current):
    
    diff = desired - current

    if(abs(diff) > 180):
        diff = diff % 180


    """
    if abs(diff) > 180:
        diff = 360 - abs(diff)
        if(diff < 0):
            diff = -diff
    """

    return diff
    
def get_angle_from_deltas(deltaX, deltaY): #tested
    
    angle = radians_to_degrees(math.atan(deltaY / deltaX))

    if(deltaX < 0):
        #Q2 and Q3
        angle += 180
    else:
        #Q1 and Q4
        angle += 360

    """
    if (deltaY < 0):
    
        if (deltaX < 0):
            # Q3
            print("Q3")
            angle = angle + 180
        
        else:
            # Q4
            print("Q4")
            angle = angle + 360
        
    elif (deltaX < 0):
        # Q2
        print("Q2")
        angle = angle + 180
    else:
        #Q1
        print("Q1")
        angle += 360
    """
    return angle - 90
    

def radians_to_degrees(radians):

    degrees = radians * (180.0 / math.pi)
    return degrees

def print_calculations(x, y):
    angle = get_angle_from_deltas(x, y)
    print("Desired angle: ", angle)

    currAngle = 350
    print("Current angle: ", currAngle)

    print("Turn ", calc_smallest_angle_difference(angle, currAngle))

""" 
while(True):
    x = input("X: ")
    y = input("y: ")

    if(x == "" or y == ""):
        break
    
    print_calculations(x, y)
    print()
 """

points = [[1, 1], [-1, 1], [-1, -1], [1, -1]]

print("Starting")
print()
for i, [x, y] in enumerate(points):
    print("Calculating ", i+1)
    print(x, y)
    print_calculations(x, y)
    print()