# SkyTrack Reminder

##  A MSFS plugin that gives a reminder to start VA flight tracking

This plugin was originally designed for [Fly UK](https://flyuk.aero) but is tracker software agnostic and will work for any VA.

### Installation Instructions

* Download the latest version from [here](https://github.com/skslater/SkyTrackReminderMSFS/releases) or compile it yourself from source.
* Extract the Zip file into "%APPDATA%\Microsoft Flight Simulator\Packages\Community" or your equivalent directory

### Usage

After turning on the main battery the plugin will display a reminder message in the centre of the screen to start your flight tracker.

The message will be repeated for jet aircraft when the first APU generator is turned on.  This can be either APU generator 1 or 2 but the message will only appear for the first one to be turned on. The message is skipped if any engines are already running when the APU generator is activated, e.g after landing.

### Known issues

Some aircraft, including the standard Boeing 747-8i, appear to incorrectly report the main battery status as on regardless of the cockpit button state. This will cause the initial reminder message to appear during the cut scene when the aircraft first reports its state.
