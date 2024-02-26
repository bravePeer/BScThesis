// pch.h: wstępnie skompilowany plik nagłówka.
// Wymienione poniżej pliki są kompilowane tylko raz, co poprawia wydajność kompilacji dla przyszłych kompilacji.
// Ma to także wpływ na wydajność funkcji IntelliSense, w tym uzupełnianie kodu i wiele funkcji przeglądania kodu.
// Jednak WSZYSTKIE wymienione tutaj pliki będą ponownie kompilowane, jeśli którykolwiek z nich zostanie zaktualizowany między kompilacjami.
// Nie dodawaj tutaj plików, które będziesz często aktualizować (obniża to korzystny wpływ na wydajność).

#ifndef PCH_H
#define PCH_H
// w tym miejscu dodaj nagłówki, które mają być wstępnie kompilowane
#include "../Desktop/board.h"
#include "../Desktop/boardSave.h"
#include "../Desktop/component.h"
#include "../Desktop/user.h"
#include "../Desktop/levelsToLoad.h"
#include "../Desktop/level.h"
#include "../Desktop/component.h"
#include "../Desktop/SHA256.h"

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>
#include "../Desktop/SimulationEngine.h"

#endif //PCH_H
