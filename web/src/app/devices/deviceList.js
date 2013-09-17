devices.controller('DeviceListCtrl', function ($scope, $location, DeviceDAO)
{
    DeviceDAO.getDevices(function (devices)
    {
        $scope.devices = devices;
    });

    $scope.stats = function (deviceId)
    {
        $location.path('/devices/' + deviceId + '/stats/');
    };
});
