import numpy
import math
import Image, ImageDraw

def find_convex_hull(points):
    def make_line(first, second):
        a = second[1] - first[1]
        b = first[0] - second[0]
        c = -a * second[0] - b * second[1]
        return (a, b, c)

    def distance(line, point):
        a, b, c = line
        return (a * point[0] + b * point[1] + c) / math.sqrt(a * a + b * b)
    
    def find_convex_hull_side(left, right):
        a, b, c = make_line(left, right)
        
        farthest = max(points, key = lambda point: distance((a, b, c), point))
        
        if abs(distance(make_line(left, right), farthest)) < 0.000001:
            return []
        else:
            return [left] + find_convex_hull_side(left, farthest) + [farthest] +\
                 find_convex_hull_side(farthest, right) + [right]
    
    left = min(points)
    right = max(points)
    
    return [left] + find_convex_hull_side(left, right) +\
        [right] + find_convex_hull_side(right, left)

def draw_point(draw, point):
    draw.ellipse([(point[0] - 3, point[1] - 3), (point[0] + 3, point[1] + 3)], fill = 0)

if __name__ == "__main__":
    points = [(numpy.random.randint(0, 1000), numpy.random.randint(0, 1000)) for i in xrange(20)]
    
    convex_hull = find_convex_hull(points)
    
    image = Image.new("RGB", (1000, 1000), (255, 255, 255))
    draw = ImageDraw.Draw(image)
    
    for point in points:
        draw_point(draw, point)
    
    draw.polygon(convex_hull, outline = 0)
    
    image.save("convex_hull.png")
    
    