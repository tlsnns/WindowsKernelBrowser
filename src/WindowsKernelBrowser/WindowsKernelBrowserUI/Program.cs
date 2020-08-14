using PInvoke;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{
    class Program
    {
        static void Main(string[] args)
        {
            DeviceDataTransmitter deviceDataTransmitter;
            var processorCount = Environment.ProcessorCount;

            if (args.Length < 1)
            {
                Console.WriteLine("请输入-h 查看使用帮助");
                return;
            }
            var arg = args[0];

            switch (arg)
            {
                case "-gdt":
                    deviceDataTransmitter = new DeviceDataTransmitter();
                    GDTFactory gDTFactory = new GDTFactory();
                    for (int i = 0; i < 1; i++)
                    {
                        GDT gdt = (GDT)gDTFactory.CreateGDT(deviceDataTransmitter, (uint)i);

                        Console.Write($"处理器号：{gdt.ProcessorNumber}=");
                        Console.Write($"gdt表基址：{gdt.TableAddress.ToString("X")}\t");
                        Console.WriteLine($"项数：{gdt.EntityCount.ToString("X")}\t");
                        gdt.GDTEntitise.ForEach(item =>
                        {
                            var keyValuePairs = item.ToDictionaries();
                            foreach (var kv in keyValuePairs)
                            {
                                Console.Write($"{kv.Key}:{kv.Value}\t");
                            }
                            Console.WriteLine();
                        });
                    }
                    deviceDataTransmitter.Close();
                    break;
                case "-idt":
                    deviceDataTransmitter = new DeviceDataTransmitter();
                    IDTFactory iDTFactory = new IDTFactory();
                    for (int i = 0; i < processorCount; i++)
                    {
                        IDT idt = (IDT)iDTFactory.CreateIDT(deviceDataTransmitter, (uint)i);

                        Console.Write($"处理器号：{idt.ProcessorNumber}\t");
                        Console.Write($"idt表基址：{idt.TableAddress.ToString("X")}\t");
                        Console.WriteLine($"项数：{idt.EntityCount.ToString("X")}\t");

                        idt.IDTEntities.ForEach(item =>
                        {
                            Console.Write($"向量号：{item.VectorNumber.ToString("X")}\t");
                            Console.Write($"段选择器：0x{item.InterruptServiceRoutineSegmentSelector.ToString("X")}\t");
                            Console.Write($"段偏移：0x{item.InterruptServiceRoutineOffset.ToString("X")}\t");
                            Console.Write($"TSS栈索引：{item.InterruptStackTableIndex}\t");
                            Console.Write($"门描述符类型：{item.TYPE}\t");
                            Console.Write($"描述符特权级别：{item.DescriptorPrivilegeLevel}\t");
                            Console.WriteLine($"标志：{item.SegmentPresentFlag}");
                        });
                    }
                    deviceDataTransmitter.Close();
                    break;
                default:
                    string strHelp =
@"-gdt
    dump gdt
-idt
    dump idt
";
                    Console.WriteLine(strHelp);
                    break;
            }
        }
    }
}
