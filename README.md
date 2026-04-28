# IoT-Based-Women-s-Safety-System

## Description
IoT-Based Women’s Safety System using ESP32 that detects emergencies via button and sends real-time GPS location alerts through GSM, initiates SOS calls, and captures images using ESP32-CAM for enhanced personal safety.

## Features
- Real-time GPS tracking
- Emergency SMS alerts
- SOS calling
- Image capture and email alert

## Components Used
- ESP32
- GPS Neo-7M Module
- SIM900A GSM Module
- ESP32 -CAM Module
- Push Button
- 1K ohm resistors
   
## Circuit Diagram
<img width="1586" height="992" alt="gitimg" src="https://github.com/user-attachments/assets/8bdb5372-1d31-4873-bf40-5d1ecb29b063" />

## How It Works
1. The project is programmed in C++ and executed using the Arduino IDE (version 1.8.16).
2.  When the alert button(push button 1) is pressed,the system sends an emergency SMS to pre-registered contacts and nearby authorities using GSM. The message includes a distress alert (“Help, I am in danger”) along with a live location link generated using GPS.
3. After sending the SMS, the LCD displays “Message Sent” to confirm successful delivery.
4. The GSM module automatically initiates a phone call to emergency contacts and police authorities to ensure immediate response even if the SMS is missed.
5. The ESP32 sends a wireless alert signal using ESP-NOW to the ESP32-CAM module. The camera captures an image of the surroundings or intruder and sends it via email using SMTP protocol.
6. If push button is pressed mistakenly , the false alert button(push button 2) should be pressed, then the system sends a message stating “Sorry, sent by mistake” to both contacts and authorities to avoid unnecessary panic.

## Output
<img width="522" height="1024" alt="image" src="https://github.com/user-attachments/assets/ddc8fd9f-74fe-44b3-9bc6-e4446f6b1aa2" />





<img width="539" height="856" alt="image" src="https://github.com/user-attachments/assets/096c1354-430b-4b6c-a581-ee534551bcce" />





<img width="575" height="407" alt="image" src="https://github.com/user-attachments/assets/eeade11b-7658-4c6e-9c0b-d456df0ecdf0" />

## Future Improvements
- Mobile app integration
- AI-based threat detection
- Wearable design
