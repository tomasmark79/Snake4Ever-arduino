# Arduino Snake4Ever

## Přehled

**Arduino Snake4Ever** je retro hra typu Snake pro Arduino, kterou můžete spustit na OLED displeji. Tento projekt je ideální pro milovníky klasických her a Arduino nadšence, kteří si chtějí vyzkoušet, jak naprogramovat hru Snake s využitím různých periferií a technologií. 

Projekt obsahuje:
- Grafiku vykreslenou na OLED displeji.
- Ovládání pomocí tlačítek.
- Zvukovou podporu tří kanálových skladeb.
- Implementovanou základní umělou inteligenci (demo režim).

### Demo režim

Projekt má integrovaný demo režim, ve kterém se had pohybuje náhodně po obrazovce. Když však uživatel stiskne tlačítko, demo režim se přeruší a začne standardní hra.

---

## Hardware Požadavky

- **Arduino** (testováno na NANO, UNO)
- **OLED displej** (SSD1306, I2C, 128x64 pixelů)
- **Tlačítka** pro ovládání (nahoru, dolů, vlevo, vpravo)
- **Reproduktor** (pro zvukový výstup MIDI hudby)
- Kabeláž a příslušenství pro připojení

---

## Použité knihovny

Tento projekt využívá následující knihovny:
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- [Playtune](https://github.com/LenShustek/arduino-playtune) (pro MIDI hudbu)

Pro instalaci těchto knihoven můžete použít Library Manager v Arduino IDE, nebo je nainstalovat ručně.

---

## Pin konfigurace

| Komponenta      | Pin       |
|-----------------|-----------|
| Tlačítko vlevo  | 2         |
| Tlačítko vpravo | 3         |
| Tlačítko nahoru | 4         |
| Tlačítko dolů   | 5         |
| OLED displej    | I2C (A4, A5 na UNO)   |
| Reproduktor     | Připojení dle potřeby |

---

## Jak hru hrát

1. Po spuštění se zobrazí demo režim, kde se had pohybuje náhodně.
2. Stiskem libovolného tlačítka demo přerušíte a začnete hrát.
3. Pohybujte hadem pomocí tlačítek (vlevo, vpravo, nahoru, dolů).
4. Vaším úkolem programátora je přidat do kódu ovoce, aby had mohl růst. **Toto je již úkol pro vás!**

---

## Úkoly pro přispěvatele

V současné verzi had neroste. Vaším úkolem jako přispěvatele je:
- Přidat do kódu funkci pro generování ovoce.
- Implementovat mechanismus růstu hada po snědení ovoce.

### Výzva:
- Jakmile dokončíte svou úpravu, pošlete Pull Request a přidejte svou funkci. Projekt čeká na vaši kreativitu!

---

## Special díky

Děkuji své úžasné přítelkyni za její podporu během tří zábavných dní, kdy jsem tento projekt vytvářel! 🤓

---

## Autor

Tomáš Mark 2024 - tomas@digitalspace.name / https://tomas.digitalspace.name

---

## License

Tento projekt je licencován jako Licence Unlicence. Nakládejte se zdrojovým kódem jak je libo.
