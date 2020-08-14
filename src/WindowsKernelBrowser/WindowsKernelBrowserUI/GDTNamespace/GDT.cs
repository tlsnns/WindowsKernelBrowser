using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{
    class GDT : KernelTable
    {
        public List<GDTEntity> GDTEntitise;
        GDT(uint processorNumber, byte[] vs, int startIndex) : base(processorNumber)
        {
            TableAddress = BitConverter.ToUInt64(vs, startIndex);//0-8
            EntityCount = BitConverter.ToInt32(vs, startIndex + 8);//8-12
            GDTEntitise = new List<GDTEntity>();
            for (int i = 0; i < EntityCount; i++)
            {
                GDTEntitise.Add(SegmentDescriptorsFactory.Create(vs, startIndex + 12 + i * 24));
            }
        }
        public static GDT Parse(uint processorNumber, byte[] vs, int startIndex)
        {
            return new GDT(processorNumber, vs, startIndex);
        }
    }
}
