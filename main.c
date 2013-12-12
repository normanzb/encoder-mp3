//    Shine is an MP3 encoder
//    Copyright (C) 1999-2000  Gabriel Bouvigne
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.

#include <stdio.h>

#include "AS3/AS3.h"

static void print_header()
{
    fprintf(stderr,"Shine 0.1.4\n");
}

int main(int argc, char **argv)
{
    print_header();

    // throw exception to interupt the run-loop, preventing the x-bridge static destructor to be executed.
    AS3_GoAsync();
}
