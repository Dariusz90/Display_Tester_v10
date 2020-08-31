# Display_Tester_v10
Firmware for 7-Seg Display Tester v1.0

Tester by Patryk B.

Założenia:

I Wariant podstawowy
1. Test wyświetlacza 7 segmentowego ze wspólną ANODĄ (DLED1)
- Zapalamy po kolei segmenty wyświetlacza, mierząc prąd (ADC_Current2)
- w trakcie trwania testu świeci jednocześnie dwukolorowa dioda D3
2. Wykrywanie błędów:
- przepalenie leda/ segmentu - nie płynie odpowiedni prąd
- zwarcia miedzy segmentami(zapalają się więcej niż 1 segment)
3. Koniec testu sygnalizowany za pomocą diody D3:
- Zielony - OK
- Czerwony - Błąd
 
II Opcjonalnie:
- SW1- wybór programu wspólną ANODA/KATODA
- możliwość testu wyświetlacza 7 segmentowego ze wspólną KATODĄ (DS1)
- LED1,LED2 - do wykorzystania
- S1,S2 - do wykorzystania - można zrobić start po wciśnięciu przycisku
