# Icarus testing

Al fine di testare le varie componenti software del progetto 'Icarus', verranno realizzati dei test di cui salveremo i risultati sottoforma di file csv con lo scopo di realizzare dei gravici tramite suddetti log.

Per avere un maggiore rigore, i vari log verranno categorizzati per agevolarne le letture e categorizzazioni.

## MPU6050
### Giroscopio (Gyroscope)
La soluzione più logica per poter analizzare in maniera ottimale i risultati consiste nello svolgere i vari test utilizzando le stesse variazioni ma cambiando l'elaborazione effettiva dei dati.

Date le suddette premesse, i test inerenti al giroscopio saranno svolti unicamente sull'asse Z.

I cambiamenti di angolo saranno i seguenti:

| Tempo  | Variazione | Valore atteso |
| :---   | :---       |    :----:     |
| 0'     | 0          | 0             |
| 30"    | +20        | 20            |
| 2'3"   | -10        | 10            |
| 3'40"  | +60        | 70            |
| 8'30"  | -20        | 50            |
| 9'30"  | -30        | 20            |
| 9'50"  | +70        | 90            |
| 10'    | -90        | 0             |


Per ogni test verranno riportati i parametri utilizzati.

#### log_20221215_1744.csv
| Parametro     | Valore        | Note                                                             |
| :---          |    :----:     | :---                                                             |
| Sampling rate | 50 Hz         |                                                                  |
| DLPF          | 21 Hz         |                                                                  |
| Approx digits | 3             | Approssimazione pre e post conversione deg-rad                   |
| Queue size    | 10            | I valori di output dello smoothing sono anch'essi approssimati   |
| Gyro range    | 500 deg/s     |                                                                  |

#### log_20221216_2352.csv
| Parametro     | Valore        | Note                                                             |
| :---          |    :----:     | :---                                                             |
| Sampling rate | 50 Hz         |                                                                  |
| DLPF          | 21 Hz         |                                                                  |
| Approx digits | 3             | Approssimazione valori di lettura del giroscopio                 |
| Queue size    | 10            | I valori di output dello smoothing sono approssimati             |
| Gyro range    | 500 deg/s     |                                                                  |

#### log_20221217_0013.csv
| Parametro     | Valore        | Note                                                             |
| :---          |    :----:     | :---                                                             |
| Sampling rate | 50 Hz         |                                                                  |
| DLPF          | 21 Hz         |                                                                  |
| Approx digits | 3             | Approssimazione pre e post conversione deg-rad                   |
| Queue size    | 10            |                                                                  |
| Gyro range    | 500 deg/s     |                                                                  |

#### log_20221217_0038.csv
| Parametro     | Valore        | Note                                                             |
| :---          |    :----:     | :---                                                             |
| Sampling rate | 50 Hz         |                                                                  |
| DLPF          | 10 Hz         |                                                                  |
| Approx digits | 3             | Approssimazione pre e post conversione deg-rad                   |
| Queue size    | 10            | I valori di output dello smoothing sono anch'essi approssimati   |
| Gyro range    | 500 deg/s     |                                                                  |


#### log_20221219_0218.csv
| Parametro     | Valore        | Note                                                             |
| :---          |    :----:     | :---                                                             |
| Sampling rate | 100 Hz        |                                                                  |
| DLPF          | 5 Hz          |                                                                  |
| Approx digits | 3             | Approssimazione pre e post conversione deg-rad                   |
| Queue size    | 10            | I valori di output dello smoothing sono anch'essi approssimati   |
| Gyro range    | 500 deg/s     |                                                                  |

#### log_20221219_0253.csv
| Parametro     | Valore        | Note                                                             |
| :---          |    :----:     | :---                                                             |
| Sampling rate | 50 Hz         |                                                                  |
| DLPF          | 5 Hz          |                                                                  |
| Approx digits | 3             | Approssimazione pre e post conversione deg-rad                   |
| Queue size    | 10            | I valori di output dello smoothing sono anch'essi approssimati   |
| Gyro range    | 500 deg/s     |                                                                  |

#### log_20221219_1615.csv
| Parametro     | Valore        | Note                                                             |
| :---          |    :----:     | :---                                                             |
| Sampling rate | 50 Hz         |                                                                  |
| DLPF          | 10 Hz         |                                                                  |
| Approx digits | 3             | Approssimazione pre e post conversione deg-rad                   |
| Queue size    | 5             | I valori di output dello smoothing sono anch'essi approssimati   |
| Gyro range    | 500 deg/s     |                                                                  |