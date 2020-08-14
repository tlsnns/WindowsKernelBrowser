using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{
    class IDT : KernelTable
    {
        public List<IDTEntity> IDTEntities;
        IDT(uint processorNumber, byte[] vs, int startIndex) : base(processorNumber)
        {
            TableAddress = BitConverter.ToUInt64(vs, startIndex);//0-8
            EntityCount = BitConverter.ToInt32(vs, startIndex + 8);//8-12

            IDTEntities = new List<IDTEntity>();
            for (int i = 0; i < EntityCount; i++)
            {
                IDTEntities.Add(IDTEntity.Parse(i, vs, startIndex + 12 + i * 14));
            }
        }
        public static IDT Parse(uint processorNumber, byte[] vs, int startIndex)
        {
            return new IDT(processorNumber, vs, startIndex);
        }
    }
}
