from machine import Pin
import time

def handle_interrupt(Pin):           #defining interrupt handling function
    global position
    global idx
    
    idx += 1
    #print(data.value(), idx)
    
    if idx < 21 :
        if data.value() == 1:
            position += 2**(idx-1)
        
    elif idx == 21:
        if data.value() == 1:
            position = -1 * position
            
    elif idx == 24:
        print(f"{position/100.00:.2f}")
        position = 0
        idx = 0
        time.sleep_us(100)   
    
data = Pin(3, Pin.IN)

idx = 0

position = 0

PIR_Interrupt=Pin(2,Pin.IN)   # setting GPIO2 PIR_Interrupt as input

PIR_Interrupt.irq(trigger=Pin.IRQ_RISING, handler=handle_interrupt)        

while True:
   pass
    
