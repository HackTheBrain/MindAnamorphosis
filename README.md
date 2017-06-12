# External Dependencies for using this project
In order to build and use this project you will need the following:
* Unreal Engine 4: https://www.unrealengine.com/en-US/blog
* Visual Studio 17 - I think this should work with the free community edition
* CMake: https://cmake.org/ - this is needed to build a 64bit version of VRPN
* VRPN: https://github.com/vrpn/vrpn/wiki - this is used to connect the BCI controller within the Unreal Project to the OpenVIBE scenario. As far as I could see only 32 bit versions of the VRPN client were available for download; and so you need to download the source code and build a 64bit version of this, as required by Unreal.
* OpenVIBE: http://openvibe.inria.fr/ - this is what is used to train and run the BCI, and to send the control commands, via VRPN, to Unreal. The set of training scenarios, and the final live scenario (BciControllerInput-v2.xml) and the training scenarios (numbered 1..3) are to be found in the 'BCI' subdirectory.

# Mind Anamorphosis
Hack The Brain Dublin 2017 Project
https://dublin.sciencegallery.com/page/mindanamorphosis

Virtual reality (VR) art allows a participant to experience disembodied presence in an immaterial, abstract space and when coupled with Brain Computer Interface it has the potential to allow a person to affect their virtual surrounding using their thoughts. We envisage the participant floating inside an immersive dreamscape VR environment, where the content of the virtual environment, visual, aural and motion can be controlled by, and responds to, the persons imagined movement. The focus will be on utilizing two waves in the alpha and beta range in primary sensorimotor cortex indicating motor intention or motor imagery of different body parts. The Mu rhythm which in the alpha range 7.5 to 12.5 Hz and the sensorimotor rhythm (SMR) in the range of 13 to 15 Hz both of which are dominant during rest but desynchronized when an individual is planning, executing, and imagining body movements. 

## Who
**Michael McMahon**
I’m a Senior Program Manager in Ericsson Mediaroom with 20 years’ experience in Software Engineering within the IPTV/VOD area. I hold a BA in Sociology, Economics and Geography, H. Dip in Arts Administration and am finishing my MSc in Software Engineering with NUI Galway. My MSc Thesis is focused on the development of an open-source BCI-VR prototype to test the manipulation of 3D objects in a Virtual Reality Environments aimed at improving motor rehabilitation outcomes. I am also a filmmaker and my short drama ‘Fiddlers Cross’ won Best Screenplay at the prestigious Flickers Rhode Island International Film Festival in 2014.

**Toby Steele**
I'm a software engineer and technology enthusiast who has spent more than 20 years designing and building software for organisations such as Sony, the BBC, Microsoft and Ericsson. I hold a BEng in Computer Systems Engineering, and whilst most of my professional experience has been related to the development of TV and Video technology, I have also worked on the development of hardware and software for music creation and artistic expression.

## Why
The inspiration for this work lies with the surrealists movement's idea of “automatic drawing” to adequately represent the unconscious mind and allow participants to step into their own painting where they might be more inclined to explore the potential personal meaning of what they find there as it responds to their thoughts – a mind Anamorphosis.

Anamorphosis is a distorted projection or perspective requiring the viewer to use special devices or occupy a specific vantage point (or both) to reconstitute the image.  Anamorphsis plays havoc with elements and principles; instead of reducing forms to their visible limits, it projects them outside themselves and distorts them so that when viewed for a cerain point or through a device they return to normal creating a poetry of abstraction, optical illusion and a philosophy of false reality.

## How
We will be using an EEG cap, electrodes, Open BCI, Openvibe, an VR DK Oculus Rift and the Unreal Game Engine.

## Ethical considerations

No PII data will be collected

Permission will be requested before any EEG data is recorded

All participant data will be destroyed after the hackathon 
