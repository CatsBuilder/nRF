/*
This software is subject to the license described in the license.txt file included with this software distribution.You may not use this file except in compliance with this license.
Copyright � Dynastream Innovations Inc. 2014
All rights reserved.
*/

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.IO;

namespace OTAUpdater
{
   /// <summary>
   /// Implements Dynastream CRC16 function
   /// </summary>
   public static class CRC
   {
      #region Methods
      public static ushort Get16(ushort crc, byte data)
      {
         ushort[] crcTable = new ushort[]
         {
            0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
            0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
         };
         ushort tmp;

         // compute checksum of lower four bits of byte
         tmp = crcTable[crc & 0xF];
         crc  = (ushort)((crc >> 4) & 0x0FFF);
         crc = (ushort)(crc ^ tmp ^ crcTable[data & 0xF]);

         // compute checksum of upper four bits of byte
         tmp = crcTable[crc & 0xF];
         crc  = (ushort)((crc >> 4) & 0x0FFF);
         crc  = (ushort)(crc ^ tmp ^ crcTable[(data >> 4) & 0xF]);

         return crc;
      }

      public static ushort Calc16(byte[] dataBlock, int size)
      {
         ushort crc = 0;

         for (int i=0; i < size; i++)
         {
            crc = CRC.Get16(crc, dataBlock[i]);
         }
         return crc;
      }
      #endregion // Methods
   }
} // namespace
