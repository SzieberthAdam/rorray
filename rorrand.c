
unsigned char rand = 0;
unsigned char rand_addbit_idx = 0;
unsigned char rand_newbit = 0;

void rand_addbit(void) // intended to be a user input callback
{
    bit = (uint8_t)(__rdtsc() & 0x01); // keep LSB (least significant bit)
    rand += bit << rand_addbit_idx;
    rand_newbit |= (1 << rand_addbit_idx);
    rand_addbit_idx += 1;
    rand_addbit_idx &= 0x07;
}