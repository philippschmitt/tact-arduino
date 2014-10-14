# Tact Serial Protocol

The Tact libraries make use of a dedicated serial communication protocol to make sensor and desktop application talk to each other.

## Syntax
Each command consists of one uppercase command key, followed by four integers for configuring the sensor, followed by a newline char. Tact responds with an integer, which is a sum of "namespace" + value.

Processing example:

    P 0 48 32 1\n
    
for humans: 

    Read Peak for sensor 0, 
    starting at position 48 of spectrum,
    read 32 measurements
    with a step-size of 1.
    
Arduino response:

    4312
    
Peak values are transmitted in a "namespace" or rather range between 4000 and 5000. Therefore, `4312-4000 = 312` is the requested peak value.


## Commands and Responses

| Data                                 | Command Key   | Response               |
| :----------------------------------- | :------------ | :--------------------- |
| **Lib Version** (initial handshake)  | V             | `1000 + VERSION`       |
| **Spectrum**                         | S             | successively:          |
| *Number of values to be transmitted* |               | `2000 + DATA_COUNT`    |
| *repeated for each value*            |               | `2976 + DATA`          |
| *transmission complete*              |               | `2975` (end)           |
| **Peak**                             | P             | `4000 + PEAK`          |
| **Bias**                             | B             | `5024 + BIAS`          |