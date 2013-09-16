var devices = angular.module('devices', ['services.DeviceDAO', 'directives.ngFocus']);

devices.config(['$routeProvider', function ($routeProvider)
{
    $routeProvider.when('/devices', {controller: 'DeviceListCtrl', templateUrl: 'app/devices/deviceList.html'}).when('/devices/:deviceId/stats',
            {controller: 'DeviceStatsCtrl', templateUrl: 'app/devices/deviceStats.html'});
}]);



