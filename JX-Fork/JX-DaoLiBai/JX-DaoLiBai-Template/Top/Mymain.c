#include "Mymain.h"
#include "AllHeader.h"

void Mymain(void)
{
    Mode_G_Setup();

    while (1)
    {
				OLED_Clear() ;
        Mode_G_Loop();

        if (curr_mode == next_mode)
        {
            switch (curr_mode)
            {
                case Mode_Null: break;
                case Mode_1: Mode_1_Loop(); break;
                case Mode_2: Mode_2_Loop(); break;
                case Mode_3: Mode_3_Loop(); break;
                case Mode_4: Mode_4_Loop(); break;
                case Mode_5: Mode_5_Loop(); break;
                case Mode_6: Mode_6_Loop(); break;
                case Mode_7: Mode_7_Loop(); break;
                case Mode_8: Mode_8_Loop(); break;
                case Mode_9: Mode_9_Loop(); break;
                case Mode_10: Mode_10_Loop(); break;
                default: break;
            }
        }
        else
        {
            switch (curr_mode)
            {
                case Mode_Null: break;
                case Mode_1: Mode_1_Exit(); break;
                case Mode_2: Mode_2_Exit(); break;
                case Mode_3: Mode_3_Exit(); break;
                case Mode_4: Mode_4_Exit(); break;
                case Mode_5: Mode_5_Exit(); break;
                case Mode_6: Mode_6_Exit(); break;
                case Mode_7: Mode_7_Exit(); break;
                case Mode_8: Mode_8_Exit(); break;
                case Mode_9: Mode_9_Exit(); break;
                case Mode_10: Mode_10_Exit(); break;
                default: break;
            }

            switch (next_mode)
            {
                case Mode_Null: break;
                case Mode_1: Mode_1_Setup(); break;
                case Mode_2: Mode_2_Setup(); break;
                case Mode_3: Mode_3_Setup(); break;
                case Mode_4: Mode_4_Setup(); break;
                case Mode_5: Mode_5_Setup(); break;
                case Mode_6: Mode_6_Setup(); break;
                case Mode_7: Mode_7_Setup(); break;
                case Mode_8: Mode_8_Setup(); break;
                case Mode_9: Mode_9_Setup(); break;
                case Mode_10: Mode_10_Setup(); break;
                default: break;
            }

            curr_mode = next_mode;
        }
				OLED_Update() ;
    }
}
