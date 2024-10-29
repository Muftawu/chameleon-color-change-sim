
## Chameleon Color Change Simulation using PID

 - This is a simple program written in C with the raylib library
 - The program mimicks the color change ability of chameleons in different environments
    
  ### OBJECTS
  - The larger circle in the center represents the chameleon and the smaller cirlce at the far right represents
  the environment or target color object.

  ### ARGS
  - You can pass some arguments to the make command. How's how they work
    
        make ARGS="fast" --> (enable fast color change/transition)
        make ARGS="output" --> (print the output of the controller process, compares the current RGB values to the Target RGB value)
        make ARGS="motion" --> (enables the chameleon to move about while still transforming the color)

        or a combination, i.e make ARGS="fast motion output"

 - ### COLOR CHANGE LOGIC

    - The implementation uses Proportional-Integral-Derivative to compute the values needed form the target color.

    - The CLITERAL (Color) utility in c [raylib] makes it easier to manipulate the color
    using the RGB and Alpha values.

    - The proportional, integral and derivative components for the R, G and B values are computed using the PID. The 
      the Alpha component of the CLITERAL utility was set to 255 to reduce the number of computations per frame.

    - The target color the to be mimicked is also represented as a CLITERAL (Color)
    
    - Computation flow (considering the Red component):
                    Proportional = (targetColor.Red - current_chameleon_color.Red)
                    Integral += Proportional * frameTime;
                    Derivative = (Proportional - previous_proportional_error) / frameTime;
    
    - This computation is run over and over again to achieve the individual values of the 'Target color'

    ## PID VALUES
        - With some experimentation I realized that these constants work quite well
            - KP = 0.66
            - KI = 0.039
            - KD = 0.0029

    ## MORE TO COME
        - I'm looking forward to using neural networks to obtain parameters for the target environment color with high 
        accuracy. 
