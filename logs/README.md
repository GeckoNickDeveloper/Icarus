# Icarus testing

Al fine di testare le varie componenti software del progetto 'Icarus', verranno realizzati dei test di cui salveremo i risultati sottoforma di file csv con lo scopo di realizzare dei gravici tramite suddetti log.

Per avere un maggiore rigore, i vari log verranno categorizzati per agevolarne le letture e categorizzazioni.

## MPU6050
### Giroscopio (Gyroscope)
La soluzione più logica per poter analizzare in maniera ottimale i risultati consiste nello svolgere i vari test utilizzando le stesse variazioni ma cambiando l'elaborazione effettiva dei dati.

Date le suddette premesse, i test inerenti al giroscopio saranno svolti unicamente sull'asse Z.

I cambiamenti di angolo saranno i seguenti:

| Variazione | Valore atteso |
| :---       |    :----:     |
| 0          | 0             |
| +20        | 20            |
| -10        | 10            |
| +60        | 70            |
| -20        | 50            |
| -30        | 20            |
| +70        | 90            |
| -90        | 0             |


Per ogni test verranno riportati i parametri utilizzati.

#### log_20221215_1744.csv
| Parametro     | Valore        | Note                                                             |
| :---          |    :----:     | :---                                                             |
| Sampling rate | 50 Hz         |                                                                  |
| DLPF          | 21 Hz         |                                                                  |
| Approx digits | 3             | Approssimazione pre e post conversione deg-rad                   |
| Queue size    | 10            | I valori di output dello smoothing sono anch'essi approssimati   |
| Gyro range    | 500 deg/s     |                                                                  |
#### log_20221219_0218.csv
#### log_20221219_02.csv
#### log_20221219_02.csv