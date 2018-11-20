20180904
========

Voy a intentar costearme el viaje a Retrozaragoza. Para ello voy a portar Cheril of the Bosque - In Another Bosque a Ordenadores de Mierda usando (y ampliando) MK3_OM.

Como no he documentado nada y tampoco recuerdo muy bien cómo se montaba (aunque Sir Ababol NESOM está reciente) voy a ir apuntando aquí lo que voy haciendo y así luego es más fácil.

Pensaba llevar las versiones de ZX y CPC a la vez pero eso implica pintar todos los sprites en pestecé lo cual puede ser un verdadero dolor, así que no sé muy bien qué hacer. Tengo todos los assets gráficos de ZX hechos, quizá lo suyo es que empiece a hacer esta versión, la deje finiquitada, y si me dan ganas me dedique al Pestecé.

~~

Lo primero es copiar utils y dev de la carpeta de Sir Ababol NESOM (versión más reciente de MK3_OM) y recrear el resto de estructura de carpetas.

Lo siguiente será generar el script que construye los assets y diseñar las estructuras de los tiestos en memoria.

Necesito añadir a mkts_om un módulo para sacar nametables comprimidas en RLE, porque quiero prescindir de bitmaps en este juego y usar sólo imagenes construidas sobre el tileset.

~~

## Loading screen

Por ahora no hay loading screen. Luego haré un dibujo chuli.

## General assets

En este juego todo son general assets:

### Sprites

Tengo el setazo de 21 cells de 16x24 de Cheril.

```
	..\utils\mkts_om.exe platform=zx mode=sprites in=..\ports\zx\gfx\ssch.png out=..\ports\zx\bin\ssch size=5,4 max=21 metasize=2,3 silent
```

Los cells de enemigos de 16x16. Los cañones son enemigos pero como son estacionarios los pintaré con tiles.

```
	..\utils\mkts_om.exe platform=zx mode=sprites in=..\ports\zx\gfx\ssen.png out=..\ports\zx\bin\ssen size=8,1 metasize=2,2 silent
```

Unos "bits" para los disparos de los cañonacos

``` 
	..\utils\mkts_om.exe platform=zx mode=sprites in=..\ports\zx\gfx\sssmall.png out=..\ports\zx\bin\sssmall size=1,1 metasize=1,1 silent
```

### Tileset

Esto incluye el metatileset (tiles de mapa, decoraciones, personajes y cañones), la fuente, y las pantallas de título y final. Lo haré mediante un script de mkts_om, pero antes tengo que apañar la conversión de nametable en RLE. O ver qué hay que pueda usar. Hay un modo `nametable` ya, pero saca un .nam de 768 bytes sin RLE ni hostias en pan. Voy a meter un modo `nametablerle` basado en él.

Creo que lo tengo pete pete :D Voy a escribir el script de conversión y le doy fran a ver qué tal va saliendo.

Tengo que hacer un visor de binarios que saque caracteres, eso molará. Y tengo que ver cómo carajo gestionaba esto los atributos,  joder.

Putos ordenadores de mierda.

A ver, hay un bgmode "zxDoBg" que parece que recorta un cacho de patrones con atributos, y luego almacena un "nametable" que escribe primero los patrones y luego los atributos. O sea, no puedo usar directamente lo de arriba porque no toma en cuenta los atributos, que van aparte en peste a rectum.

Joder, dos días y ya echo de menos las consolas.

Lo suyo sería comprimir, en ZX, el resultado de zxDoBg (que escribe en mainBin los patrones y en auxBin la pseudonametable).

Vale, tengo que hacer uso de zxDoBg si en el script piden "nametable" o "nametablerle". Ya veré qué carajo hago con el modo directo.

zxDoBg deja en auxBin todo el nametable. Eso lo copio y tal y cual y pascual. A ver como me lo montoro.

```bas
	zxDoBg (img As Any Ptr, xc0 As Integer, yc0 As Integer, w As Integer, h As Integer)
```

Vale, resuelto. Ahora zxDoNametable llama a zxDoBg y copia de auxBin en nametable. Ese nametable se puede escribir fácilmente desde el script y además puede comprimirse (zxDoNametableRle llama a su vez a zxDoNametable; puedes hacer un "output nametablerle" desde el script).

Voy a escribir el script, ejecutarlo, y luego a escribir un visualizador de binarios para peste a rectum que muestre los patrones en una rejilla de 16x16.

Vamos a probar con este script:

```
	# Creates the full tileset to be preloaded at the main binary.
	# Exports a set of tilemaps and related metadata

	reset patterns
	reset tmaps

	# Font, 64 patterns

	open font.png
	charset 0, 0, 32, 2

	# Fixed screens

	open title.png
	nametablerle
	output nametablerle ../bin/title.rle.bin

	open ending.png
	nametablerle
	output nametablerle ../bin/ending.rle.bin

	# Metatileset

	open ts.png
	defaultink 7
	metatileset 0, 0, 16, 3, 2, 2
	output tmaps ../bin/ts.tilemaps.bin
	reset tmaps

	# Done

	stats
	output patterns ../bin/ts.bin
```

~~

¡Qué desastre! En las nametables hay mierda pura. Y luego todo lo demás falla, clar. Para colmo, me dice que está recortando 500 y nosecuantos patrones XD

Ay mi madre. Revisemos.

~~

Vale. zxDoBg es problemático porque COPIA las nametables al binario principal. No me sirve. Tengo que hacer una función específica (que será muy sencilla).

~~

Vale, los resultados que obtengo ahora tienen algo más de sentido XD

Tengo que reducir buscando redundancia porque me salen 268 patrones y tengo que bajar a 256 (12 menos). Voy a darle fran. Pero antes me quiero hacer el visor (adapto chrview).

### Assets de nivel

Mapa: packed, 16 tiles (este comprime mal):

```
	..\utils\packmap_sp.exe in=..\map\level0.map out=..\ports\zx\bin\ size=5,7 tlock=15 fixmappy nodecos noindex
```

Enemigos:

```
	..\utils\eneexp3c_sp.exe in=..\enems\level0.ene out=..\ports\zx\bin\ yadjust=1
```

Behs

```
	..\utils\list2bin.exe  ..\ports\zx\gfx\ts.behs ..\ports\zx\bin\behs.bin
```

## Librarian

El librarian tiene sentido con cosas comprimidas y tal. Creo que me lo voy a fumar para este juego.

~~

Ahora voy a 

# Montar los binarios en el heap!

El "heap" es como he llamado a donde va toda la ñorda binaria externa, o sea, el archivo `ram/heap.h`. Aquí es donde lo montaré todo.

```c
#asm
	;; Patterns and tilemaps for the metatileset

#ifdef SPECCY
	._ts
		BINARY "../ports/zx/bin/ts.bin" ; 2048 bytes
#endif
#ifdef CPC 
		; _ts and tiles are defined and XREFd in tilemap_conf.asm	
#endif		

	._tsmaps
#ifdef SPECCY
		BINARY "../ports/zx/bin/ts.tilemaps.bin" ; 384 bytes
#endif
#ifdef CPC
		BINARY "../ports/cpc/bin/ts.tilemaps.bin" ; 384 byte
#endif		

	._behs
#ifdef SPECCY
		BINARY "../ports/zx/bin/behs.bin"
#endif
#ifdef CPC
		BINARY "../ports/cpc/bin/behs.bin"
#endif


	;; Sprite cells

#ifdef SPECCY
	._ss
		; Dummy 8 empty (masked) spaces for vertical rotation
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
		defb 0, 255
#endif		
	._ss_main
#ifdef SPECCY		
		; 21 cells, 192 bytes each
		BINARY "../ports/zx/bin/ssch.bin"
#endif		
#ifdef CPC
		; 21 cells, 96 bytes each
		BINARY "../ports/cpc/bin/ssch.bin"
#endif		

	._ss_enems
#ifdef SPECCY		
		; defs SPRITES_EXTRA_CELLS * 144
		; 8 cells, 144 bytes each
		BINARY "../ports/zx/bin/ssen.bin"
#endif
#ifdef CPC
		; defs SPRITES_EXTRA_CELLS * 64
		; 8 cells, 64 bytes each
		BINARY "../ports/cpc/bin/ssen.bin"
#endif		

	._ss_extra

	._ss_small
#ifdef SPECCY
		; defs SPRITES_SMALL_CELLS * 64
		; 1 cell, 64 bytes
		BINARY "../ports/zx/bin/sssmall.bin"
#endif
#ifdef CPC
		; defs SPRITES_SMALL_CELLS * 64
		; 1 cell, 64 bytes
		BINARY "../ports/cpc/bin/sssmall.bin"
#endif
#ifdef CPC
	._ss_empty
		; a 100% transparent sprite
		BINARY "../ports/cpc/bin/ssempty.bin"
#endif		

	;; Map data + index

	._map
#ifdef SPECCY
		BINARY "../ports/zx/bin/map.bin"
#endif
#ifdef CPC
		BINARY "../ports/cpc/bin/map.bin"
#endif

#ifdef HOTSPOT_TYPE_KEY
	._locks
#ifdef SPECCY		
		BINARY "../ports/zx/bin/locks.bin"
#endif
#ifdef CPC
		BINARY "../ports/cpc/bin/locks.bin"
#endif
#endif

	;; Enems data + index

	._enems
#ifdef SPECCY
		BINARY "../ports/zx/bin/enems.bin"
#endif
#ifdef CPC
		BINARY "../ports/cpc/bin/enems.bin"
#endif		

	;; Hotspots

	._hotspots
#ifdef SPECCY
		BINARY "../ports/zx/bin/hotspots.bin"
#endif
#ifdef CPC
		BINARY "../ports/cpc/bin/hotspots.bin"
#endif		
#endasm

```

## Datos de tamaños y cantidades y hostias

Empezamos con `definitions.h`:

Número de sprites software (no cells, entidades)

```c
	#define SW_SPRITES_16x24				1		// # of 16x24 software sprites
	#define SW_SPRITES_16x16				3		// # of 16x16 software sprites
	#define SW_SPRITES_16x8					0		// # of 16x8 software sprites
	#define SW_SPRITES_8x16					0		// # of 8x16 software sprites
	#define SW_SPRITES_8x8					1		// # of 8x8 software sprites
```

Dentro de las entidades (en total 4+1 = 5) donde empieza cada tipo. Player es el 0, enemigos son 1, 2, 3. El coco es el 4.

```c
	#define SPR_PLAYER						0
	#define SPR_ENEMS_BASE					1
	#define SPR_HITTER						0xff
	#define SPR_COCOS_BASE					4
```

Sobre el heap:

```c
	// About the heap
	#define SPRITES_EXTRA_CELLS				8		// # of 16x16 sprite cells besides the player (for enemies, etc)
	#define SPRITES_SMALL_CELLS				1		// # of 8x8 sprite cells (sword, punch, bullets...)
```

Configuración del juego:

```c
	#define MAX_PANTS						35
	[...]
	#define BOLTS_MAX 						4	
	[...]
	#define LIFE_INI						25
	#define LIFE_REFILL						5
```

```c
	#define TALL_PLAYER						// Player is 16x24
```

Screen size

```c
	#define MAP_WIDTH_16					// Screens are 16 tiles wide. Undef for 15.
	#define MAP_HEIGHT_12					// Screens are 12 tiles high (hack). Undef for 10.
```

Cell base for enemies. Tenemos 21 cells para Cheril, así que:

```c
	#define ENEMS_CELL_BASE					21
```

Valor más bajo de Y. Como tenemos 2 filas de tiles de marcador arriba y el personaje es de 16 (logico) de alto, 

```c
	#ifdef SPECCY
		#define ABSOLUTE_BOTTOM	0
	#endif
	#ifdef CPC
		#define ABSOLUTE_BOTTOM 0 // 16
	#endif
```

Aunque ya lo revisaré cuando empiece el port de pestecé.

```c
	#define PLAYER_GENITAL
	#define FLOATY_PUSH_BOXES
	#define PLAYER_PUSH_BOXES
	#define PLAYER_FIRE_TO_PUSH

	#define PLAYER_REBOUNDS
```

Cells

```c
	#define CELL_FACING_RIGHT 				0
	#define CELL_FACING_LEFT				5
	#define CELL_FACING_DOWN				10
	#define CELL_FACING_UP					15
	#define CELL_IDLE						0
	#define CELL_WALK_BASE					1
	#define CELL_HURT						20
```

## Configuraciones varias que no recuerdo.

Voy examinando los archivos y apuntando por aquí :-D

### `assets/levelset.h`

Apuntar en los includes al sitio correcto donde están los archivos enems.h y hotspots.h para los niveles que tenga el juego (en este caso, sólo uno).

```c
	#ifdef SPECCY
		#include "..\ports\zx\bin\enems.h"
		#include "..\ports\zx\bin\hotspots.h"
	#endif
	#ifdef CPC
		#include "..\ports\cpc\bin\enems.h"
		#include "..\ports\cpc\bin\hotspots.h"
	#endif
```

Luego datos sobre el nivel. No uso scripting, pero tampoco molesta (por ahora):

```c
#define SCRIPT_INDEX		0
#define PLAYER_INI_X		1
#define PLAYER_INI_Y		2
#define PLAYER_SCR_INI		3
#define LEVEL_MAP_W			4
#define MAX_HOTSPOTS_TYPE_1	5

const unsigned char level0[] = {
	0,
	3, 3, 
	15,
	5,
	MAX_HOTSPOTS_0_TYPE_1
};
```

### `assets/spriteset.h`

Vamos al lío tapatío. Esta puede dar miedow. Vamos a encargarnos por ahora sólo de la parte peste a rectum, dejemos la pestecé para después (a riesgo de volerme locow).

Número de cells de cada tamaño

```c
	#define MAX_CELLS_16x24		21
	#define MAX_CELLS_16x16 	8
	#define MAX_CELLS_16x8		0
	#define MAX_CELLS_8x16		0
	#define MAX_CELLS_8x8		1
```

Esto pone un puntero a cada cell (dentro del heap) en el array `sprite_cells`. Creo que ya le voy cogiendo el rollo.

```c
	void sprite_cells_init (void) {
		rda = 0; 

		// 24x24 cells
		gp_gen = ss_main; gpit = MAX_CELLS_16x24; while (gpit --) {
			sprite_cells [rda ++] = gp_gen;
			gp_gen += 192;
		}

		// 16x16 cells
		gp_gen = ss_enems; gpit = MAX_CELLS_16x16; while (gpit --) {
			sprite_cells [rda ++] = gp_gen;
			gp_gen += 144;
		}

		// 8x8 cells
		gp_gen = ss_small; gpit = MAX_CELLS_8x8; while (gpit --) {
			sprite_cells [rda ++] = gp_gen;
			gp_gen += 64;
		}
	}
```

### `util/system_speccy.h`

Tendré que adaptar esto para meter los cells de 16x24 y tal. Creo que lo he hecho bien.

### `engine/printer_speccy.h`

Tengo que dar soporte a dibujar pantallas de 16x12 con la primera y última filas comías (o sea, pintando semitiles)

Ahora me acabo de dar cuenta de que esto tienía buffers de 160 bytes. Espero que me quepan los de 192 :-S Creo que lo he apañado XD

### util/general.h

Cambio `map_base_address` para estas pantallas de 192 tiles.

~~

Bueno, compilado todo, resulta que se cuelga, como era de esperar. Ahora toca ponerse a ver dónde :-D

Por lo pronto llega a `game_loop`, pasando por `system_init`, `sprite_cells_init`, `level_setup` y `game_init`. Veamos donde nos quedamos...

Tengo un cuelgue fast and furious después de incluir `engine/mainloop/flick_screen.h`, que seguramente sea llamando al cambio de pantalla en `game_prepare_screen`...

Se cuelga justo después de `draw_scr` de la misma forma que después de incluir todo el `flick_screen.h`, así que el problema grave debe estar por el momento ahí. Esto está bien porque es una de las cosas que he tocado :*)

Voy a mostrar la pantalla (`sp_UpdateNow`) cada vez que pinte un tile a ver qué pasa.

La pantalla que se muestra es rara: parece correcta (alineada), ¡pero los tiles impares salen corruptos! Además, al pintar la linea inferior es cuando se cuelga. Probablemente mi código que pinta el semitile superior en la linea más inferior esté fallando. Vale, está al revés :*)

Ahora ya pinta la pantalla completa (mal, tiles impares mal), y se cuelga luego. Voy a resolver la pantalla y luego veo el cuelgue. 

En los tiles pares lee de `gp_tmap` en rdct y se queda con el nibble izquierdo. En los tiles impares extrae el nibble derecho del rdct leído anteriormente. Seguramente algo se lo esté follando en el proceso, pero no encuentro qué.

¿Qué misterioso agente estará corrompiendo el valor de rdct? Los únicos arrays que veo antes de rdct (basatnte arriba) son cosas de spriterío que no se están usando en este proceso :-/

Voy a alterar el algoritmo a ver.

```c
	rdct = *gp_map ++;
	rdt = rdct >> 4; advance_worm ();
	rdt = rdct & 0x0f; advance_worm ();
```

Mismo resultado. rdct corrupto.

No me gusta esto, pero voy a probar así a ver:

```c
	rdt = (*gp_map) >> 4; advance_worm ();
	rdt = (*gp_map) & 0x0f; advance_worm ();
	gp_map ++;
```

Así lo pinta bien... ¿Pero qué corrompe rdct? Es raro porque se usa en todos los demás métodos de dibujado.

Sigamos buscando el cuelgue.

Ahora ya se pasa todo el `game_prepare_screen`.

Vale, esto me pasa siempre. Se para en `CONTROLLER_READ`. Probablemente no habré definido nada :*) Añado esto en `game_title`:

```c
	gpit = 1; controls_setup ();
```

OK! Ya no se cuelga, pero los sprites de los enemigos están corruptos. De hecho parece que todos los sprites están o mal generados o mal ubicados. Con paciencia :-)

Revisemos el cálculo de los punteros que apuntan a los cells. Un visor de binarios con sprites tampoco vendría mal.

Las cuentas no me fallan: los sprites de 16x24 son 192 bytes: 3 columnas de 4 bloques. Cada bloque son 16 bytes, por lo tanto 3 columnas de 64 bytes = 192 bytes.

Los sprites sw parecen bien creados además. Todo tiene el tamaño correcto.

Mirando el binario de Cheril parece que también está todo correcto. Los datos generados corresponden con lo que se espera.

- Los facings están bien.
- Pero al animar hace cosas muy raras.

Están pasando cosas raras all around. Seguramente haya corrupción de datos. No es normal que yo haga (variable & 1) y el resultado sean bonitos números (debería ser 0 ó 1).

Veamos: memoria reservada:

```c
	sp_AddMemory (0, NUMBLOCKS, 14, AD_FREE);
```

`NUMBLOCKS` vale `(((SW_SPRITES_16x16*13)+(SW_SPRITES_16x16*10)+(SW_SPRITES_8x8*5)+((SW_SPRITES_16x8+SW_SPRITES_8x16)*7)))`

Vale, ya he visto que eso por lo pronto está mal. Hay una constante mal puesta. Arreglado eso tengo:

```c
	#define NUMBLOCKS (((SW_SPRITES_16x24*13)+(SW_SPRITES_16x16*10)+(SW_SPRITES_8x8*5)+((SW_SPRITES_16x8+SW_SPRITES_8x16)*7)))
	#define AD_FREE	  (RAMTOP-NUMBLOCKS*15)
```


`NUMBLOCKS` es `1 * 13 + 3 * 10 + 3 * 5 + (0+0) * 7 = 13 + 30 + 15 = 58`. No parece mucho ¿no? Vamos, que he tenido más.

`AD_FREE = RAMTOP - 58*15`. ¿Cuánto vale `RAMTOP`? Vale 61440.

`61440 - 58 * 15 = 61440 - 870 = 60570`. Supuestamente el binario ocupa ahora 30997 y se compila en 24000, therefore llega hasta 54997, lo que aú nme deja unos 5K de margen, hulio.

¿Qué copón pasa? Lo siguiente a mirar es que los buffers que meto por entre medias de mierdas y hostias quepan. Veamos `globals.h`

```c
	unsigned char scr_buff [SCR_BUFFER_SIZE] @ FREEPOOL;
	unsigned char scr_attr [SCR_BUFFER_SIZE] @ (FREEPOOL + SCR_BUFFER_SIZE);
```

¿Donde tengo ese `FREEPOOL`? 

```c
	#define TEXTBUFF		23296
	#define BASE_EP 		23296 + 116
	#define FREEPOOL 		23296 + 116 + 252
	#define BASE_BREAKABLE	23296 + 116 + 252 + 320
```

Todo eso mina la memoria por abajo. El que daría problemas es la ubicación de BASE_BREAKABLE que no cabría con los 320 que le dejo de los dos buffers de 160 (ahora sería 384). En ese caso `23296 + 116 + 252 + 384 = 24048` pero eso no me daría problemas ahora.

Vamos, que tiene que ser otra cosa.

Pero es que es para flipar que haga

```c
	sp_Border (frame_counter & 1);
```

Y el borde se vaya a todos los colores O_o

# Todo

[/] Render de pantallas de 16x12 (y sprites en su sitio)
	[X] ZX
	[ ] CPC
[X] Steady shooters empleando tiles.

20180905
========

Voy a necesitar más gráficos (estoy haciendo los steady shooters y van por tiles), así que voy a crear dos sets: uno para cutscenes y otro para juego. En realidad tres binarios:

- Fuente.
- Cutscenes.
- Ingame.

Eo. No. Podría hacerlo, pero me jodería toda la portabilidad. A menos que en Pestecé haga las pantallas de otra forma, lo que haría que no cupiese nada... Nah, tengo que ganar 16 caracteres como sea. 

~~

[ ] Se queda atrapada en los evil tiles. BOF.

[X] Reducir más el tamaño haciendo la extracción a globales también en `enems_load`.

Ahora: 32218 -> 32170. Tengo que sacar más ariables: x1, x2, y1, y2. -> 32054. Bueno hulio, ALGO ES ALGO.

~~

Ahora es cuando meto a los personajes. No sé qué he hecho, pero meter el hud ha pillado unos 300 bytes. Es para flipar el z88dk, macho. Por eso antes de meter a los personajes me tengo que poner a limpiar y sacar la basura :-/ O sea, recortar bytes.

~~

### Quitar parámetros de `p_t2`

32314 -> 32312. GUAU

Mentira, 33275, que no había metido las pantallas comprimidas. Creo que además está metiendo el musicazo beeper, y es una baza que tengo en la manga, pero por ahora lo dejo así para vivir el peligro, que si no, no me inspiro.

### Vamos a cambiar a RLE44 y puedo jugar con...

hacer el mapa menos complejo. Ahora estamos en 33452 (map=3360). Simplemente cambiando a RLE44: 33128 (map=3158). Aquí ya puedo empezar a recortar.

Reduciendo el mapa: 3158 -> 3147 -> 3107.

Voy a poner todos los ladrillos iguales y los cambio por random. Y lo mismo con las hojas.

Tile ladrillo: 2 (2/4)
Tile hojas: 3 (3/8)
Tile suelo: 11 (11/20)
Tile seta: 5 (5/21)
Mapa: 2812, 2740

Voy a escribir el custom. 32765 y muy bonito :)

Estamos en 32684. Voy a hacer por meterle los diálogos y los personajes. Estudiemos como está hecho en el original y veamos si hay una forma más mejor y menos peor.

~~

1.- Shuffle. Hay que "barajar" el array de orden para que sea diferente cada partida:

```c
	gpit = 5; while (gpit --) character_order [gpit] = gpit + 1;
	gpit = 10; while (gpit --) {
		rda = rand8 () & 7; if (rda > 4) rda -= 5;
		rdb = rand8 () & 7; if (rdb > 4) rdb -= 5;
		rdc = character_order [rda]; character_order [rda] = character_order [rdb]; character_order [rdb] = rdc;
	}
	character_order_idx = 0;
```

2.- Aparte, tenemos `ob_char_n_pants` con la pantalla donde aparece cada personaje, y `ob_char_yx` con las coordenadas. `ob_char_x` y `ob_char_y` para detectar colisión y tal luego.

```c
	const unsigned char ob_char_n_pants [] = {
		15, 5, 30, 4, 29, 20
	};

	const unsigned char ob_char_yx [] = {
		0x4c, 0xa5, 0x44, 0x2d, 0x1c, 0x48
	};
```

Los tiles empleados para el personaje "i" son `32 + i * 2` y el siguiente. 

Vamos a empezar pintando a los personajes.

~~

Seguimos: 

3.- Los textos y scripts están en `text_scripts.h` y `texts.h`. Copio el contenido a mi `ob_data.h`.

Ahora me he quedado en 34693. El margen se reduce rápido. En pestecé no sé qué voy a pestecer.

~~

4.- Cuando interactuamos con un personaje N se sigue este algoritmo:

- `character_order_idx` dice el orden de hablar. Puede valer 0, 1, 2, 3, 4, 5. 

- Si vale 5, es que ya hemos hablado con todos. En ese caso, todos los personajes dicen lo mismo: las lineas 8 y 9: 'YOU"RE DONE WITH US' / 'COLLECT THE NUTS!'.

- Si vale menos que 5, 
	- Si el personaje con el que tenemos que hablar (`character_order [character_order_idx]`) es el de esta pantalla (`ob_char_id`) o hablamos con johnny limite (`ob_char_id == 0`):
		- ejecutar el script `ob_char_id`
		- sumar 10 de vida
		- incrementar `character_order_idx`.
	- Si no, dice "SOY ..."

	- En cualquier caso, dice "Ahora tienes que hablar con ..."

20180906
========

Lista de cosas que hacer / cambiar / mejorar / reparar:

[X] Cuando empujas un bloque puedes pisar el hotspot. El hotspot sigue ahí para cogerlo, pero desparece para siempre. Se puede solucionar repintando el hotspot siempre que se mueva un bloque.

[ ] Hacer que los bloques movidos puedan matar a los enemigos ¿cabrá? Lo dejamos para el final y si eso ponemos más bloques.

[ ] Recolocar enemigos. Muchos están puestos por poner o están muy cerca de los bordes. Probar también la optimización de establecer un clipping rectangle individual para cada sprite.

[X] Los cactus se ven muy poco. Rebordear y recolorear.

[X] Las setas alt no matan - revisar behs.

[X] Los cañones verticales no funcionan , o no funcionan por estar en el borde? 

[X] Los personajes se les habla pondiendose abajo solo. Cambiar la detección para tomar un area mas grande: 8 pixels a la izquierda y derecha y 8 pixels arriba.

~~

Sobre los cañones, he visto más miserias. Seguramente llamar a `set_map_tile` joda toda la marrana por estar usando rd*. Debería desde ya:

[ ] Cambiar `set_map_tile` y todas las demás para usar el set _x, _y, _t, _n. 

[ ] ?

Vamos al lío.

Tras reorganizar un poco las globales para que todo sea más mejor y menos peor, ya aparecen todos los cañones, pero las direcciones verticales siguen sin funcionar. Revisemos los cocos.

Joder, era una tontería pero me ha costao: Tenía definido COCOS_MAX como 2 en vez de 3.

~~

Ahora tengo que programar el descompresor RLE y creo que me he metido en contramano. Estoy almacenando un RLE con los números de patrones y luego los atributos... ¿Pero cómo pinto yo eso con splib2? No puedo tocar sus estructuras a mano (creo) - aunque eso sería lo suyo.

No aparece ninguna mención al buffer en el mapa de memoria, por lo que debe ser una estructura estática interna de splib2. A menos que la exporte de alguna manera y recompile splib2 será invisible!

Voy a ver si tengo por aquí el código de splib2.

~~

¡Lo descubrí! Cómo cambiar los atributos en el buffer:

```asm
	    LIB SPCompDListAddr
	    ld a, 0	; row
	    ld c, 0 ; col
	    ld d, 56 ; attr
	    ld e, 10 ; tile
	    push de 
	    call SPCompDListAddr
	    pop de
	    ld (hl), d ; Write attribute
	    inc hl
	    ld (hl), e ; Write char
```

Y así queda:

```c

	void unrle_adv (void) {
		gpint --;
		if (gpint > 767) {
			//sp_PrintAtInv (_y, _x, 71, rdb);
			#asm
				LIB SPCompDListAddr
				ld a, (__x)
				ld c, a
				ld a, (_rdb)
				ld d, a
				ld a, (__y)
				push de 
				call SPCompDListAddr 
				pop de 
				inc hl
				ld (hl), d
			#endasm
		} else {
			if (gpint == 767) { _x = _y = 0; }
			#asm
				LIB SPCompDListAddr
				ld a, (__x)
				ld c, a
				ld a, (_rdb)
				ld d, a
				ld a, (__y)
				push de 
				call SPCompDListAddr 
				pop de 
				ld (hl), d
			#endasm
		}
		_x ++; if (_x == 32) { _x = 0; _y ++; }	
	}

	void unrle (void) {
		// Point gp_aux to rle'd data.
		rdc = *gp_aux ++;	// run code
		_x = 0; _y = 0; rdb = 0; gpint = 1536;
		while (gpint) {
			rda = *gp_aux ++;
			if (rda == rdc) {
				rda = *gp_aux ++;
				if (rda == 0) break;
				while (rda --) unrle_adv ();
			} else {			
				rdb = rda;
				unrle_adv ();
			}
		}
	}
```

Ole me :D

~~

Faltaba invalidar el rectángulo, pero eso era cosa fásil. Ahora voy a recolocar enemigos y ver si mejoro un poco el mapa.

20180907
========

Está hecho. Obviamente necesita pulir aristas, pero básicamente está hecho. Ahora a ver la versión de pestecé. Esos sprites me dan miedo total ¿Y si soy un barato y adapto los de CherilP? Porque convertir + retocar los existentes puede ser la muerte. Ya lo intenté, pero creo que no conservo los resultados.

20180910
========

ZX On, CPC empezada. Gráficos, etc, todo hecho. Montando binario. Por ahora compila y no se cuelga aunque mil cosas están mal y tengo que implementar otras mil. Por lo pronto el binario se compila a 1024 y ocupa 28763. Arkos va a 34868 y las musicas se descomprimen en 32250, por lo que por ahora vamos bien de espacio.

Veo que en el renderer solo se muestran los tiles < 16, y eso me resulta raro. Voy a imprimir todo el tileset a ver.

20180911
========

Medio va, pero mal. No sé si reparar primero, o ponerme a rehacer las rutinas PutSpTileMapXxY para quitar bucles, ganar algún ciclo, y simplificarlas.

Ahora mismo sale gráfico pesturris en vez de proyectiles, pero al menos parece que no se cuelga...

~~

Ok, eran todo fallos mierder míos y tan estúpidos... En serio, siempre pasa igual.

Ahora hay otro glitch porque Cheril aparece con trozos fuera de la pantalla al entrar por abajo en una nueva habitación. Esto de no tener clipping es lo pior. Voy a ver cuánto se come a ver si puedo apañar algo.

Tengo definido PRY_MAX = 192. Este es el valor de Y en el que detecta el cambio de habitación y al que establece Y al cambiar.

Hay que tener en cuenta que el espacio para sprites es 16 pixels más alto que la pantalla, que maneja un área de 208 píxels de alto que empieza en el 24 y muestra (en OM) 176 pixels hasta 200. Si no me fallan los cálculos, escribir un sprite de 24 (que va con ajuste Y=-8) en 192 muestra sólo las 16 primeras lineas. Eso lo puedo apañar :-)

Y luego voy a reescribir las rutinas de CPCRSLIB que "ensucian" los tiles que pisan los sprites para eliminar bucles y hacerlos más rápidos. Algo ganaré.



