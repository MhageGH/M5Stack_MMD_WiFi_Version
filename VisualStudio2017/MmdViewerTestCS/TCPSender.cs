﻿using System;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;

namespace MmdViewerTestCS
{
    public class TCPSender
    {
        private Task _task;
        private string _addr;
        private int _port;
        private byte[] _sendBuffer;
        private CancellationTokenSource _cts;
        private float[] rotation;

        public TCPSender(float[] rotation)
        {
            this.rotation = rotation;
        }

        public void start(string addr, int port)
        {
            _addr = addr;
            _port = port;
            _cts = new CancellationTokenSource();
            _task = Task.Run(() => { TaskClient(_cts.Token); });
        }

        public void stop()
        {
            _cts?.Cancel();
        }


        public void setData(byte[] src)
        {
            _sendBuffer = src;
        }

        private void TaskClient(CancellationToken ct)
        {
            byte[] resBytes = new byte[256];
            while (!ct.IsCancellationRequested)
            {
                try
                {
                    var client = new TcpClient(_addr, _port);
                    var ns = client.GetStream();
                    client.NoDelay = true;
                    ns.ReadTimeout = 2000;
                    ns.WriteTimeout = 2000;
                    var nodatacount = 0;
                    while (!ct.IsCancellationRequested && client.Connected)
                    {
                        try
                        {
                            var res = ns.Read(resBytes, 0, resBytes.Length);
                            if (0 < res)
                            {
                                nodatacount = 0;
                                if (    resBytes[0] == 0x4A  // "JPG"
                                     && resBytes[1] == 0x50
                                     && resBytes[2] == 0x47
                                     && resBytes[3] == 0x0A)
                                {
                                    var s = System.Text.Encoding.UTF8.GetString(resBytes, 0, res);
                                    s = s.Replace("\n", "");
                                    var ss = s.Split(new char[] { ' ' });
                                    if (ss.Length == rotation.Length + 1) for (int i = 0; i < rotation.Length; ++i) rotation[i] = (float)Convert.ToDouble(ss[i + 1]);

                                    var tmp = _sendBuffer;
                                    ns.Write(tmp, 0, tmp.Length);
                                }
                                else
                                {
                                    Console.WriteLine("unknown cmd");
                                }
                            }
                            else
                            {
                                Thread.Sleep(1);
                                if (++nodatacount > 1000) break;
                            }
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine(ex.Message);
                        }
                    }
                    ns.Close();
                    client.Close();
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                }
            }
        }
    }
}
