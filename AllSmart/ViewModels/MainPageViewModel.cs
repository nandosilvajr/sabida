using System.Windows.Input;
using AllSmart.Services;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Microsoft.AspNetCore.Components;

namespace AllSmart.ViewModels;

public partial class MainPageViewModel : BaseViewModel
{


    [ObservableProperty] private int _sensorData;

    [Inject] private IWebServiceApi WebServiceApi { get; set; }
    public MainPageViewModel()
    {
    }
}