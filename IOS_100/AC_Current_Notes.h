/*
 * Convert maximum RMS current to peak-current by multiplying by √2.
     Primary peak current = √2 x Primary RMS_Peak_current
     1.414 x 30 A = 42.42A

  Divide the Primary_peak-current by the number of turns in the CT to give the peak-current in the secondary coil
    The YHDC SCT-013-000 CT has 2000 turns, so the secondary peak current will be:
    Secondary peak current = Primary peak current / no. of turns
    42.42A / 2000 = 0.02121A

  To maximise measurement resolution, the voltage across the burden resistor at peak-current should = 0.5 of AREF ~1V
    Ideal load resistance = (AREF/2) / Secondary peak-current
    0.500V / 0.02121A = 23.57Ω
    0.482V / 0.02121A = 22.72Ω
    Settle on 22Ω on the short side


  Calibration = CT Ratio / Burden resistance 
  (30A / 0.02121A) / 22 Ohms = 64.29

    More cray here https://learn.openenergymonitor.org/electricity-monitoring/ctac/calibration

 * 
 * 
 * 
 * 
 * 
 * 
 */
