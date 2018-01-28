/*
 * flash.c
 *
 *  Created on: Jan 10, 2018
 *      Author: Nick Sterenberg
 */

#include "main.h"
#include "stm32f4xx_hal.h"
#include "flash.h"
#include "commandline.h"

extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;

uint8_t LOG_OPEN = 0;

// eh

uint16_t two_one(uint8_t *arr){
  uint16_t out =  (*(arr) << 8) | *(arr+1);
  return out;
}
uint8_t *one_two(uint16_t in){
  static uint8_t out[2];
  out[0] = in >> 8;
  out[1] = in;
  return &out[0];
}
uint16_t packet_length(uint8_t packet_type){
  switch(packet_type){
    case PACKET_TYPE_MILLIS_TIMESTAMP:
      return PACKET_LENGTH_MILLIS_TIMESTAMP;
    case PACKET_TYPE_MICROS_TIMESTAMP:
      return PACKET_LENGTH_MICROS_TIMESTAMP;
    case PACKET_TYPE_GYRO:
      return PACKET_LENGTH_GYRO;
    case PACKET_TYPE_ACCEL:
      return PACKET_LENGTH_ACCEL;

  }
}

void read_filesystem(filesystem* f){
  load_page(0);
  read_buffer(0, (uint8_t*) f, sizeof(filesystem));
}
void write_filesystem(filesystem* f){
  erase_block(0);
  load_page(0);
  write_buffer(0, (uint8_t*) f, sizeof(filesystem));
  program_page(0);
}

// eh




// Loads a page into the flash page buffer (on the fash chip)
// TODO: test
uint16_t load_page(uint16_t page_number){

  uint8_t data[4];
  data[0] = FLASH_COMMAND_PAGE_READ;
  data[2] = (page_number & 0xFF00) >> 8;
  data[3] = page_number & 0x00FF;

  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 0);
  HAL_SPI_Transmit(&hspi1, data, 4, 0xFF);
  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 1);

  while(flash_busy());      // wait for the page to load

}

// Sends a one-byte flash commad to the chip
// TODO: None
void flash_command(uint8_t command){
  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 0);
  HAL_SPI_Transmit(&hspi1, &command, 1, 0xFF);
  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 1);
}

// Checks the flash ID matches data sheet. A good check to see if the chip is alive
int read_flash_id(){


  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 0);
  uint8_t data[8];
  data[0] = 0x9F;
  HAL_SPI_Transmit(&hspi1, data, 1, 0xFF);
  HAL_SPI_Receive(&hspi1, data, 4, 0xFF);
  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 1);

  if(data[1] == 0xef && data[2] == 0xaa && data[3] == 0x21){
      return 0;
  }
  else{
      return 1;
  }

}
void unlock_all(){
  flash_command(FLASH_COMMAND_WRITE_ENABLE);
  uint8_t data[3];
  data[0] = FLASH_COMMAND_WRITE_STATUS_REGISTER;
  data[1] = 0xA0;
  data[2] = 0x00;
  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 0);
  HAL_SPI_Transmit(&hspi1, data, 3, 0xFF);
  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 1);
}


void read_buffer(uint16_t column, uint8_t *buffer, uint16_t size){

  uint8_t data[4];
  data[0] = FLASH_COMMAND_READ_DATA;
  data[1] = (column & 0xFF00) >> 8;
  data[2] = column & 0x00FF;

  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 0);
  HAL_SPI_Transmit(&hspi1, data, 4, 0xff);

  HAL_SPI_Receive(&hspi1, buffer, size, 0xff);

  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 1);

}

void erase_block(uint16_t block_number){

  flash_command(FLASH_COMMAND_WRITE_ENABLE);
  uint8_t data[4];
  data[0] = FLASH_COMMAND_BLOCK_ERASE;
  data[2] = (block_number & 0xFF00) >> 8;
  data[3] = block_number & 0x00FF;

  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 0);
  HAL_SPI_Transmit(&hspi1, data, 4, 0xFF);
  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 1);

  while(flash_busy());

}

void write_buffer(uint16_t column, uint8_t *page_buffer, uint16_t size){


  flash_command(FLASH_COMMAND_WRITE_ENABLE);
  uint8_t data[3];
  data[0] = FLASH_COMMAND_LOAD_RANDOM_DATA;
  data[1] = (column & 0xFF00) >> 8;
  data[2] = column & 0x00FF;

  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 0);
  HAL_SPI_Transmit(&hspi1, data, 3, 0xff);

  HAL_SPI_Transmit(&hspi1, page_buffer, size, 0xff);

  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 1);

}
uint8_t flash_read_status_register(uint8_t reg){
  uint8_t data[2];
  data[0] = FLASH_COMMAND_READ_STATUS_REGISTER;
  data[1] = reg;

  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 0);
  HAL_SPI_Transmit(&hspi1, data, 2, 0xFF);
  HAL_SPI_Receive(&hspi1, data, 1, 0xFF);
  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 1);

  return data[0];
}
void program_page(uint16_t page_number){

  flash_command(FLASH_COMMAND_WRITE_ENABLE);
  uint8_t data[3];
  data[0] = FLASH_COMMAND_PROGRAM_EXECUTE;
  data[2] = (page_number & 0xFF00) >> 8;
  data[3] = page_number & 0x00FF;

  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 0);
  HAL_SPI_Transmit(&hspi1, data, 4, 0xff);
  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 1);

  while(flash_busy());    // Wait for the page to finish programming


}
uint8_t flash_busy(){
  return flash_read_status_register(0xC0) & 0b00000001;
}
uint8_t flash_test(){


}
file *new_log(){


  file* log = malloc(sizeof(file));
  filesystem tempfs;
  read_filesystem(&tempfs);
  tempfs.num_files += 1;

  log->start_page = tempfs.next_file_page;
  log->current_page = log->start_page;
  load_page(log->current_page);
  log->bytes_free = 2048;
  log->file_number = tempfs.num_files - 1;

  tempfs.files[log->file_number] = *log;
  write_filesystem(&tempfs);

  return log;
}
uint32_t log(file* f, uint8_t *data, uint32_t length){

  if(length > f->bytes_free - 1){
      uint8_t eof = PACKET_TYPE_EOP;
      write_buffer(2048-(f->bytes_free), &eof, 1);
      program_page(f->current_page);
      load_page(++f->current_page);
      f->bytes_free = 2048;
  }

  write_buffer(2048-(f->bytes_free), data, length);
  f->bytes_free -= length;

}
void log_gyro(file* f, gyro* g){
  uint8_t data[PACKET_LENGTH_GYRO+1];
  data[0] = PACKET_TYPE_GYRO;
  data[1] = g->id;
  for(int n = 0; n < 3; n++){
      data[2*n+2] = (g->data[n] & 0xff00) >> 8;
      data[2*n+3] = (g->data[n] & 0x00ff);
  }
  log(f, data, sizeof(data));
}

uint32_t close_log(file *f){
  uint8_t eof = PACKET_TYPE_EOP;
  write_buffer(2048-(f->bytes_free), &eof, 1);
  program_page(f->current_page);
  f->stop_page = f->current_page;
  filesystem tempfs;
  read_filesystem(&tempfs);
  tempfs.files[f->file_number]=  *f;
  write_filesystem(&tempfs);
  free(f);
  return 0;
}

void print_file(uint32_t filenum){
  filesystem tempfs;
  read_filesystem(&tempfs);

  gyro g;
  accel a;
  baro b;
  uint32_t time = 0;

  uint8_t line[255];
  snprintf(line, sizeof(line), "Time(ms), byte, gyro x, gyro y, gyro z, accel x, accel y, accel z, barodata,\r\n\0");
  HAL_UART_Transmit(&huart1, line, strlen(line), 0xff);

  uint16_t current_page = tempfs.files[filenum].start_page;
  while(current_page < tempfs.files[filenum].stop_page){
      uint8_t page[2048];
      load_page(current_page);
      read_buffer(0, page, 2048);
      uint8_t type = page[0];
      int32_t base_index = 0;
      while(type != PACKET_TYPE_EOP){
          switch(type){
            case PACKET_TYPE_GYRO:
                g.id = page[base_index+1];
                g.data[0] = page[base_index+2] << 8;
                g.data[0] |= page[base_index+3];
                g.data[1] = page[base_index+4] << 8;
                g.data[1] |= page[base_index+5];
                g.data[2] = page[base_index+6] << 8;
                g.data[2] |= page[base_index+7];
                base_index += 8;
              break;
            case PACKET_TYPE_ACCEL:
              a.data[0] = page[base_index+1] << 16;
              a.data[0] |= page[base_index+2] << 8;
              a.data[0] |= page[base_index+3];
              a.data[1] = page[base_index+4] << 16;
              a.data[1] = page[base_index+5] << 8;
              a.data[1] |= page[base_index+6];
              a.data[2] = page[base_index+7] << 16;
              a.data[2] |= page[base_index+8] << 8;
              a.data[2] |= page[base_index+9];
              base_index += 10;
              break;
            case PACKET_TYPE_BARO:
              b.data = page[base_index+1] << 24;
              b.data |= page[base_index+2] << 16;
              b.data |= page[base_index+3] << 8;
              b.data |= page[base_index+4];
              base_index += 5;
              break;
            default:
              break;
          }
          type = page[base_index];
          snprintf(line, sizeof(line), "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n\0", time, base_index, g.data[0],
                   g.data[1], g.data[2], a.data[0], a.data[1], a.data[2], b.data);
          HAL_UART_Transmit(&huart1, line, strlen(line), 0xff);
      }
      current_page++;
  }

}
void print_file_raw(uint32_t filenum){
  filesystem tempfs;
  read_filesystem(&tempfs);
  uint16_t start = tempfs.files[filenum].start_page;
  uint16_t stop = tempfs.files[filenum].stop_page;
  for(int n  = start; n <= stop; n++){
      load_page(n);
      uint8_t buffer[2048];
      read_buffer(0, buffer, 2048);
      HAL_UART_Transmit(&huart1, buffer, 2048, 0xff);
  }
}
