# Satisfactory ModLog Library
Custom Modding Library to allow writing clean external log files

Rather than finding your mod issues in the UE5 log file, this allows you to output to a custom logfile that's easier for your to parse.

Includes blueprint callable logging functions as well, including the ability to override the log output to revert back to using UE_LOG so you can change the behavior of logging before packaging yuor mod.

It has some configuration in the header file you will need to do before it works

If you have feedback/improvements, please leave a pull request! This is very much prototype at this stage and there's a lot that can be improved.

Example "Clean" log output that goes to an external logfile.
```
Resource Roulette Logging initialized
Sam | Pure: 0 | Normal: 0 | Impure: 1
Sam | Pure: 0 | Normal: 0 | Impure: 2
Sam | Pure: 1 | Normal: 0 | Impure: 2
Iron | Pure: 0 | Normal: 1 | Impure: 0
Iron | Pure: 0 | Normal: 2 | Impure: 0
Iron | Pure: 0 | Normal: 3 | Impure: 0
Iron | Pure: 0 | Normal: 4 | Impure: 0
Iron | Pure: 0 | Normal: 5 | Impure: 0
Copper | Pure: 1 | Normal: 2 | Impure: 0
Stone | Pure: 3 | Normal: 4 | Impure: 0
Resource nodes updated successfully.
```
