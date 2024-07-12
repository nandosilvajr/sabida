using AllSmart.Services;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace AllSmart.ViewModels;

public partial class MainPageViewModel : BaseViewModel
{
    private readonly IWebServiceApi _webServiceApi;

    [ObservableProperty] private int _sensorData;
    [ObservableProperty] private bool _serviceStateOn;
    
    [RelayCommand]
    async void Refresh()
    {
        IsRefreshing = false;
        await GetDeviceState();
    }
    
    public MainPageViewModel(IWebServiceApi webServiceApi)
    {
        _webServiceApi = webServiceApi;
    }
    
    public async Task<bool?>GetDeviceState()
    {
        IsBusy = true;

        try
        {
            var response = await _webServiceApi.GetSysInfo();
            
            IsBusy = false;
            
            return ServiceStateOn = response is not null;
        }
        catch (Exception e)
        {
            IsBusy = false;
            
            await Application.Current.MainPage.DisplayAlert(
                "Device not found!",
                "Check if the device is connected.",
                "Ok");
            
            return ServiceStateOn = false;
        }
        finally
        {
            IsBusy = false;
        } 
    }
}