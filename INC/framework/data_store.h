/**
 ******************************************************************************
 * @file    data_store.h
 * @author  KCE R&D Team
 * @date    2024-11-29
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_DATA_STORE_H_ 
#ifdef INC_DATA_STORE_H_

/* Enum type definitions -----------------------------------------------------*/


/* typedef definitions -----------------------------------------------------*/

/* function protocol -----------------------------------------------*/
void original_to_reset(void);
void eepread_to_systable(void);
void systable_to_eeprom(uint8_t addr);
void test_datastore(void);

#endif /* INC_DATA_STORE_H_ */