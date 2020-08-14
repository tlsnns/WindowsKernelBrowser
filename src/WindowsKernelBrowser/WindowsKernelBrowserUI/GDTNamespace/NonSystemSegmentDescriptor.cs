using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WindowsKernelBrowserUI.GDTNamespace;

namespace WindowsKernelBrowserUI
{
    abstract class NonSystemSegmentDescriptor : GDTEntity
    {
        public bool Accessed { get; private set; }
        public NonSystemSegmentTypes NonSystemSegmentType { get; private set; }

        public NonSystemSegmentDescriptor(byte[] vs, int startIndex) : base(vs, startIndex)
        {
            Accessed = BitConverter.ToBoolean(vs, startIndex + 13);//13-1

            NonSystemSegmentType = (NonSystemSegmentTypes)vs[startIndex + 16];//16-1
        }
        public override Dictionary<string, string> ToDictionaries()
        {
            var d = base.ToDictionaries();
            d.Add("NonSystemSegmentType", NonSystemSegmentType.ToString());
            d.Add("Accessed", Accessed.ToString());
            return d;
        }
    }
}
