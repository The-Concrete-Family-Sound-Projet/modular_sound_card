# modular_sound_card
*Open Hardware modular soundcard and audio player using a Raspberry Pi and Pure Data*


The purpose of this project is to realize a very easy to use, transportable and open hardware audio player and sound card.
The idea is to have a very compact piece of hardware being the player (from an analog or digital input) sending I2S on 16 outputs connected to DACs wich will then send the audio to speakers.
The particularity of this device will be the treatment of the sound totally controlled softwarly.

Open hardware solutions able to deal with sound are quite limited today, three of them are emerging, Axoloti, Bela and Raspberry Pi. 
We choose to begin with the Raspberry Pi for it’s user-friendly interface and to associate it with Pure Data.

![Rasp-pd-out](https://i.imgur.com/dnFZhXI.png)

## Pure Data & WiringPi

In order to have full control over your Raspberry Pi GPIO with Pure Data (Pd), a library called WiringPi is available. It allows you to send any digital information you what on your GPIO pins from Pd.

With this solution, you can imagine a very wide range of application. Having treatment handled on Pd to apply filters, change the volume of tracks separately, ect…

We didn’t found a Pd i2s object for the moment, if you know one don’t hesitate to leave us a comment. Otherwise we will need to create it ourself.

## Creating i2s on Pure Data

i2s is the most adapted and also most used communication protocol for digital audio application and allows stereo transmission.
It is composed of 3 wires, a global clock (BCLK), a second clock 32 times slower to indicate the right or left channel (LRCK) and a Data wire (DOUT).

![Imgur](https://i.imgur.com/Uc3VMHv.jpg)

The LRCK clock is running at 48KHz, during its high level, the 16 bits of the Data represent the right channel and low level the 16 bits of left channel. The BCLK is running at 1,536MHz in order to send the Data flow.

We want to send the audio acquired from a digital or analog input to a i2s output.

First we need to convert the audio file to a number to fit digital output needs using the [snapshot~] object. It will need to make the converting at a speed of 96KHz (twice faster than LRCK to convert once Right channel and then Left channel).
The Data output from i2s send binary in two's complement at the speed of the BCLK so we need to split the number into 16 binary that will be sent one by one. The 16 bits are seperated regarding the LRCK but one bit after the rising or falling edge.

We also need to create the 2 clocks synchronized with the Data wire flow.

Because of the lack of simple object realizing what we want (we could do it with a lot of simple blocks but it would slow the system), we need to create one in C language.

## Pure Data Externals in C

Creating my own objects in C is not an easy task, it is not much documented.
I found two tutorials, the pure-data/externals-howto GitHub project and the cheetomoskeeto/pd-externals-c GitHub with a series of 11 Youtube videos.

Following this exemples we tried to begin by a simple [through~] object that just output the audio signal given at the input.
After creating this object, we want to move forward to the purpose of the project and create in a second time two objects.

The first one will convert the input of type signal into a 16 bits integer output written in two's complements (the i2s data format).
The other needs to output a binary signal from a 16 bits integer at a given frequency (to have a 48kHz frequency on our i2s output we need to write binary at a speed of 1,536MHz).

Once we will have this 2 objects, we want to end up with a global object able to generate i2s from a signal type message in pure data. 

We hope this project achieved to attract your interest and a little help to continue would be very appreciated :D
