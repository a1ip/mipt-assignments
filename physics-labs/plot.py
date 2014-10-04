import math

def make_plot(input, output, x_name, y_name, calculator):
    input_file = open(input, 'r')
    output_file = open(output, 'w')

    output_file.write('%s\t%s' % (x_name, y_name))
    output_file.write('\t%s\t%s\n' % (x_name + '-er', y_name + '-er'))

    for line in input_file:
        data = line.split()
        data = map(lambda x: float(x), data)

        x, y, xe, ye = calculator(data)

        output_file.write('%f\t%f' % (x, y))
        output_file.write('\t%f\t%f\n' % (xe, ye))

    input_file.close()
    output_file.close()

if __name__ == '__main__':
    def calculator(data):
        x = data[0] + data[1]
        y = data[2] + data[3]
        xe = 1.0
        ye = 20.0
        return (x, y, xe, ye)
        
    make_plot('input', 'output', 'x', 'y', calculator)
