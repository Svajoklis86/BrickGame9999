#include "GSMenu.hpp"

void GSMenu::defineGraphics()
{
	for (int i = 0; i < letterCount; i++)
	{
		letters[i] = "";
	}

	letters[A] =
		"  *  "
		" * * "
		"*   *"
		"*****"
		"*   *";

	letters[B] =
		"**** "
		"*   *"
		"**** "
		"*   *"
		"**** ";

	letters[C] =
		" *** "
		"*   *"
		"*    "
		"*   *"
		" *** ";

	letters[R] =
		"**** "
		" *  *"
		" *  *"
		"**** "
		" *  *";

	for (int i = 0; i < numCount; i++)
	{
		numbers[i] = "";
	}

	numbers[0] =
		"***"
		"* *"
		"* *"
		"* *"
		"***";

	numbers[1] =
		" * "
		"** "
		" * "
		" * "
		"***";

	numbers[2] =
		"***"
		"  *"
		"***"
		"*  "
		"***";

	numbers[3] =
		"***"
		"  *"
		"***"
		"  *"
		"***";

	numbers[4] =
		"* *"
		"* *"
		"***"
		"  *"
		"  *";

	numbers[5] =
		"***"
		"*  "
		"***"
		"  *"
		"***";

	numbers[6] =
		"***"
		"*  "
		"***"
		"* *"
		"***";

	numbers[7] =
		"***"
		"  *"
		"  *"
		" * "
		" * ";

	numbers[8] =
		"***"
		"* *"
		"***"
		"* *"
		"***";

	numbers[9] =
		"***"
		"* *"
		"***"
		"  *"
		"***";


	anims[A][0] =
		"        "
		"    *   "
		"        "
		"        "
		"   *    "
		"   *    "
		" ***    "
		"*****   ";

	anims[A][1] =
		"        "
		"    *   "
		"        "
		"        "
		"   **   "
		"   **   "
		" ****   "
		"   **   ";

	anims[A][2] =
		"        "
		"   **   "
		"    *   "
		"    *   "
		"   **   "
		"   **   "
		" ***    "
		"        ";

	anims[A][3] =
		"        "
		" ****   "
		" *  *   "
		"        "
		"   *    "
		"   *    "
		" ***    "
		"        ";



	anims[B][0] =
		"        "
		"    *   "
		"        "
		"        "
		"        "
		"        "
		"        "
		"*****   ";

	anims[B][1] =
		"        "
		"    *   "
		"        "
		"        "
		"    *   "
		"    *   "
		"    *   "
		"   **   ";

	anims[B][2] =
		"        "
		"   **   "
		"    *   "
		"    *   "
		"    *   "
		"    *   "
		"        "
		"        ";

	anims[B][3] =
		"        "
		"   **   "
		"   **   "
		"   *    "
		"   *    "
		"        "
		"        "
		"        ";

	anims[C][0] =
		"        "
		" ****** "
		" ****** "
		" ****** "
		"        "
		"        "
		"   *    "
		" ****   ";

	anims[C][1] =
		"        "
		" ****** "
		" ****** "
		" **** * "
		"     *  "
		"        "
		"        "
		"  ****  ";

	anims[C][2] =
		"        "
		" ****** "
		" ****** "
		" **** * "
		"        "
		"        "
		"       *"
		"    ****";

	anims[C][3] =
		"        "
		"********"
		" ****** "
		"***** **"
		"     *  "
		"        "
		"        "
		"   **** ";


	anims[R][0] =
		"  *   * "
		" *  *   "
		"   *    "
		" *   *  "
		"  *     "
		"     *  "
		"   *    "
		" *    * ";

	anims[R][1] =
		"     *  "
		"*  *   *"
		"  *  *  "
		"*   *   "
		"  *   * "
		"   *    "
		"*     * "
		"    *   ";

	anims[R][2] =
		"  *  *  "
		"      * "
		" *  *   "
		"   *  * "
		" *   *  "
		"   *   *"
		"*   *   "
		" *     *";

	anims[R][3] =
		"*    *  "
		"   *  * "
		"*      *"
		"  *  *  "
		"    *  *"
		"  *   * "
		"*   *   "
		" *   *  ";
}