# Senex_Public
## What is Senex VR?
The Senex Suit, a flagship product of SenexVR, was designed to track body motion and simulate lifelike interactions in the virtual world. It provides high-fidelity data (relative position/orientation to under 0.2cm) while remaining inexpensive and straightforward to use. Senex has also developed high-accuracy, real-time dead-reckoning modules capable of indoor and outdoor translational position tracking with an accuracy of under 3cm. 

<img width="1440" alt="Gif" src="https://github.com/Eemac/Senex_Public/assets/28767801/af33710f-d304-48ee-82d3-273f5d365b34">

The idea of a wearable computer/control suit has been rolling around in my head since I saw Iron Man when I was nine. The abysmal experience of Zoom classrooms in 2020 (my sophomore year of high school) inspired me to create something better, and I’ve spent thousands of hours over nearly four years creating the full-body system (suit, gloves, and tracker base stations) I have today.

## What is it used for?
The Suit and its accompanying software package are currently geared toward diagnosing patheokinematic (irregular) movement that may lead to injury in first responders, but the potential use cases are endless. Below is a screenshot of a 45-second plank test, one of a few working 'application modules' I've built out in both hardware and software.  
<img width="1440" alt="Plank Test" src="https://github.com/Eemac/Senex_Public/assets/28767801/418c651f-eccb-40e1-b5a6-51703c11411d">


## What is included in this Repository?
The technology behind Senex VR and the Senex Suit is not currently public (Patent Pending), but I've provided some C++ header files and a compressed IMU hex image that give an idea of what I've been working on for the past four years.

## Some Hardware
|      A Glove   |  Render of a Base Station Tracker
:-------------------------:|:-------------------------:
![IMG_7187](https://github.com/Eemac/Senex_Public/assets/28767801/1a0fc150-1266-49a3-a65b-dc39c434e1ba) | <img width="630" alt="Screenshot 2023-11-21 at 4 30 07 PM" src="https://github.com/Eemac/Senex_Public/assets/28767801/bb1a0065-933a-4f3a-961c-f895bfe86317">


## The Flex PCB Panel - Created using Autodesk Eagle's Education edition
I sent this panel, a collection of over sixty flexible 2-layer printed circuits, to be manufactured the summer of my Sophomore year - about the time when COVID-19 struck. My abysmal online experience became my biggest source of motivation--I wanted to create technology that would allow me to interact with my friends, family, and school again! If the experience had to become virtual, I wanted something much more realistic than a video call. There was an upside to 'Zoom School' as well: I was able to devote many more hours to the suit than I would have been able to normally.

![board_layout](https://github.com/Eemac/Senex_Public/assets/28767801/c3999841-08fe-45d8-a377-75e836a36c2c)
