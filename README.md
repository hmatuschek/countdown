# Coundown
A very simple timer/count-down application. 

<img src="http://i58.tinypic.com/5cdnc7.png" border="0">
<img src="http://i57.tinypic.com/2m7d9qf.png" border="0">

No one wears wristwatches anymore. Hence this application is intended for the use in tests and exams, 
to display the remaining time to the students/pupils. It (optionally) plays a sound and changes the 
color of the clock once the "last minutes", i.e. the last 5min of the exam starts. 

The application itself resides in the system tray and the clock display can be hidden. This allows to
use the timer for the ["Pomodoro Technique"](https://en.wikipedia.org/wiki/Pomodoro_Technique).

The timer configurations are organized in independent "profiles". This allows to store and apply 
frequently used configurations easily.


# Features

  * Clock display can be configured to be shown in full screen, "always on top" or hidden.
  * Configurable clock display scheme and colors.
  * Configurable sounds.
  * Support of an arbitrary number of "profiles". This allows


# Install

Windows and MacOS X binaries can be obtained from the 
[releases](https://github.com/hmatuschek/countdown/releases) page. Binary packages for Ubutnu are available at my [PPA](https://launchpad.net/~hmatuschek/+archive/ubuntu/ppa). You can add this repository to your list of package sources to receive automatic updates on new releases of the software. 

To add the PPA simply call 
```
sudo apt-add-repository ppa:hmatuschek/ppa
sudo apt-get update
```

Then you can install `countdown` by calling
```
sudo apt-get install countdown
```


# Licence

countdown - A simple timer application. Copyright (C) 2015 Hannes Matuschek

This program is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software Foundation;
either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
this program; if not, write to the 
Free Software Foundation, Inc., 
51 Franklin Street, Fifth Floor, 
Boston, MA 02110-1301, USA.
