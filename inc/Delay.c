void Delay(unsigned int ms)
{
    unsigned char i, j;
    while (ms--)
    {
        i = 2;
        j = 234;
        do
        {
            while (--j)
                ;
        } while (--i);
    }
}