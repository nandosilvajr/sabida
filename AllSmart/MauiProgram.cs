using AllSmart.Services;
using AllSmart.ViewModels;
using Microsoft.Extensions.Logging;
using MudBlazor.Services;
using Refit;

namespace AllSmart;

public static class MauiProgram
{
    public static MauiApp CreateMauiApp()
    {
        var builder = MauiApp.CreateBuilder();
        builder
            .UseMauiApp<App>()
            .ConfigureFonts(fonts => { fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular"); });

        builder.Services.AddMauiBlazorWebView();

        builder.Services
            .AddRefitClient<IWebServiceApi>()
            .ConfigureHttpClient(
                c => c.BaseAddress = new Uri("http://webserver")
                )
            .ConfigureHttpClient(
                c => c.Timeout = TimeSpan.FromSeconds(10)
            );

        builder.Services.AddMudServices();
        builder.Services.AddSingleton<MainPageViewModel>();
        builder.Services.AddTransient<MainPage>();

#if DEBUG
        builder.Services.AddBlazorWebViewDeveloperTools();
        builder.Logging.AddDebug();
#endif

        return builder.Build();
    }
}