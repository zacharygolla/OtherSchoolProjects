Alright so to run php files youll need to go to wampserver.com/en/ and download the 64 bit version.

Once youve done that I would go to https://www.activestate.com/komodo-ide and download the 21 day free trial.

Ok, this part will get a little confusing.

Go to control panel>system>advance system settings>Environment Variables

Theres gonna be a top and bottom section:
Top Section: User variables for 'your user'
Bottom Section: System Variables

In both sections you will select the path option by double clicking it.
Once selected you will type C:\wamp64\bin\php\php7.2.10

Click Ok

In the bottom section (system variables) add the path under ComSpec as well (same method).
**Im not sure if you have to do the comspec one, but the video I watched did it, so I did too.

Once you do that open your command prompt.

Im pretty sure that you have to be in the same directory as where you have the file for the file to run

So when I open command prompt I have C:\Users\Zac
This is where I also have the Text-1.php file saved.

As long as you ahve file saved there the command in command prompt is php Text-1.php

Any questions just text me.