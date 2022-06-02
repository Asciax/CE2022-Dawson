import serial.tools.list_ports





def fetch_ports():
    available_ports = serial.tools.list_ports.comports()
    port_list = []
    for port in available_ports:
        port_list.append(str(port))
    return port_list


def manual_select_port():
    port_number = int(input("What is the port number? "))
    port_list = fetch_ports()
    for i in port_list:
        if i.startswith("COM"):
            if i[0:4] == (f"COM{port_number}"):
                return f"COM{port_number}"
            else:
                print("This specific COM port is not open!")
                return None
        else:
            print("No COM ports open!")
            return None


def auto_select_port():
    port_list = fetch_ports()
    if len(port_list) == 0:
        print("No ports open!")
    else:
        for port in port_list:
            if port.startswith("COM"):
                if "Arduino Uno" in port:
                    return port[0:4]
            else:
                manual_select_port()


def assign_port(port, arduino):
    if port is not None:
        arduino.port = port
    else:
        print("Failed to assign a port!")
        return None


#print(auto_select_port())
#print(manual_select_port())


def Port(Arduino):
    assign_port(auto_select_port(), Arduino)
    #print(Arduino.port)

if __name__ == '__main__':
    #Arduino = serial.Serial(baudrate=9600, timeout=0.1)
    Port()
