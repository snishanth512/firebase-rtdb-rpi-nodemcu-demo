from gpiozero import CPUTemperature
import psutil
import time
from firebase import firebase

import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO_TRIG = 21
GPIO_ECHO = 20
GPIO.setup(GPIO_TRIG, GPIO.OUT)
GPIO.setup(GPIO_ECHO, GPIO.IN)

firebase = firebase.FirebaseApplication('https://iiot-57d9d-default-rtdb.firebaseio.com/', None)

while True:
    cpuTp = CPUTemperature().temperature
    cpuCore =  psutil.cpu_percent(interval=None, percpu=True)

    GPIO.output(GPIO_TRIG, GPIO.LOW)
    time.sleep(2)
    GPIO.output(GPIO_TRIG, GPIO.HIGH)
    time.sleep(0.00001)
    GPIO.output(GPIO_TRIG, GPIO.LOW)

    while GPIO.input(GPIO_ECHO)==0:
        start_time = time.time()

    while GPIO.input(GPIO_ECHO)==1:
        Bounce_back_time = time.time()

    pulse_duration = Bounce_back_time - start_time
    distance = round(pulse_duration * 17150, 2)

    print ("cpu temperature: ",cpuTp)
    print ("Per CPU usage in percent",psutil.cpu_percent(interval=None, percpu=True))
    print('memory % used: ', psutil.virtual_memory()[2])
    print ('Per CPU frequency: ',psutil.cpu_freq(percpu=True))
    print("\n\n")

    firebase.post("/raspberry-pi-2/health-monitor/cpu-temp", cpuTp)
    firebase.post("/raspberry-pi-2/sensors/ultrasonic", distance)
    firebase.post("/raspberry-pi-2/health-monitor/cpu-core0-usage", cpuCore[0])
    firebase.post("/raspberry-pi-2/health-monitor/cpu-core1-usage", cpuCore[1])
    firebase.post("/raspberry-pi-2/health-monitor/cpu-core2-usage", cpuCore[2])
    firebase.post("/raspberry-pi-2/health-monitor/cpu-core3-usage", cpuCore[3])

    time.sleep(20)
