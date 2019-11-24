using System;
using System.IO;
using System.Windows;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Hosting;

namespace Mbcsh5view
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    // ReSharper disable once RedundantExtendsListEntry
    public partial class App : Application
    {
        internal static ILoggerFactory LoggerFactory;
        private readonly IHost host;

        public App()
        {
            host = new HostBuilder()
                .ConfigureHostConfiguration(configHost =>
                {
                    configHost
                        .SetBasePath(Directory.GetCurrentDirectory())
                        .AddJsonFile("Mbcsh5view.json", optional: false);
                })
                .ConfigureLogging((hostContext, configLogging) =>
                {
                    configLogging
                        .AddConfiguration(hostContext.Configuration.GetSection("Logging"))
                        .AddConsole()
                        .AddDebug();
                })
                .ConfigureServices((hostContext, services) =>
                {
                    services.AddOptions();
                    services.AddSingleton<Contents>();
                })
                .Build();
            LoggerFactory = host.Services.GetService<ILoggerFactory>();
        }

        private async void ApplicationStartup(object sender, StartupEventArgs e)
        {
            //     StartupUri="Contents.xaml"
            await host.StartAsync();

            var mainWindow = host.Services.GetService<Contents>();
            mainWindow.Show();
        }

        private async void ApplicationExit(object sender, ExitEventArgs e)
        {
            using (host)
            {
                await host.StopAsync(TimeSpan.FromSeconds(5));
            }
        }
    }
}
