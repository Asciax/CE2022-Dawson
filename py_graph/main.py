import matplotlib.animation
import serial
import portsModule
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
sns.set_style("darkgrid")

#csv_header = ['index', 'temp', 'humid']
'''
def next_items(d_list):
    if len(d_list) == 0:
        next_temp = None
        next_humid = None
    elif len(d_list) == 1:
        next_temp = d_list[0]
        next_humid = None
    else:
        next_temp = d_list[-2]
        next_humid = d_list[-1]
    return next_temp, next_humid
'''
temp_list = []
humid_list = []
float_list = []

gtemp_val =[]
ghumid_val =[]

# Initializing the PySerial object Arduino
Arduino = serial.Serial(baudrate=9600, timeout=0.1)

#Assigning a port to the Arduino object
portsModule.Port(Arduino)

#Opening the Serial port
Arduino.open()

fig, ax = plt.subplots(2, tight_layout = True, figsize= (12,8))
fig.suptitle("Temperature and Humidity of Remote Sensor", weight = 700, fontproperties = {'family' : 'sans-serif', 'size' : 'x-large', 'stretch' : 500})




def graph_list_update() :
    if (len(temp_list) != 0) and (len(humid_list) != 0):
        gtemp_val.append(temp_list[-1])
        ghumid_val.append(humid_list[-1])


def lists_sorting(float_l):
    if len(float_l) == 0:
        pass
    elif len(float_l) == 1:
        temp_list.append(float_l[0])
    else:
        temp_list.append(float_l[-1])
        humid_list.append(float_l[-2])


check = [False]


def graph_animate(i):
    global counter
    if check[0] == False:
        # Variable used to write data only once both temp and humid have been received
        counter = 0
        check[0] = True


    # index += 1
    incoming_message = Arduino.readline().decode('utf-8').rstrip()
    if len(incoming_message) != 0:
        print(incoming_message)
        try:
            float_list.append(float(incoming_message))

        except ValueError:
            pass
        print(float_list)

        if (counter % 2 == 0):
            lists_sorting(float_list)

        print(temp_list)
        print(humid_list)

        graph_list_update()

        print("GRAPH DATA")
        print(gtemp_val)
        print(ghumid_val)



        #CLearing the axis such that the line does not keep changing color.
        ax[0].cla()
        ax[1].cla()

        ax[0].set_title(label = 'Recorded Live Temperature')
        ax[1].set_title(label = "Recorded Live Percentage of Humidity")

        ax[0].set_xlim(xmin =0)
        ax[0].set_ylim(top=100)

        ax[1].set_xlim(xmin = 0)
        ax[1].set_ylim(top = 100)

        ax[0].set_yticks(np.arange(0,101,5))
        ax[1].set_yticks(np.arange(0, 101, 5))

        ax[0].set_xticks(np.arange(0, 101, 5))
        ax[1].set_xticks(np.arange(0, 101, 5))

        ax[0].minorticks_on()
        ax[1].minorticks_on()


        ax[0].set_ylabel('Temperature (Celsius)')
        ax[1].set_ylabel('Humidity %')

        ax[0].plot(gtemp_val, color = 'k')
        ax[1].plot(ghumid_val, color= 'k')
        plt.pause(0.001)

        counter += 1

        return fig







def main():

    # Remnants from csv attempt.
    #csv_file = open("data.csv", 'w')
    #csv_writer = csv.DictWriter(csv_file, fieldnames=csv_header)
    #csv_writer.writeheader()

    animation = matplotlib.animation.FuncAnimation(plt.gcf(), graph_animate, interval=1)
    plt.show()


    '''
        # Remnants from the csv idea.
        if counter % 2 == 0:
            next_temp, next_humid = next_items(float_list)

            csv_writer.writerow(
                {
                    'index': index,
                    'temp': next_temp,
                    'humid': next_humid
                }
            )
    '''


if __name__ == "__main__":
    main()
