devices.controller('DeviceStatsCtrl', function ($scope, $routeParams, $timeout, DeviceDAO)
{
    $scope.dateOptions = {
        'year-format': "'yy'",
        'starting-day': 1
    };

    var STAT_TYPE_AVG = "avg";
    var STAT_TYPE_MIN = "min";
    var STAT_TYPE_MAX = "max";

    $scope.startDate = new Date();
    $scope.endDate = new Date();
    $scope.startDateOpened = false;
    $scope.endDateOpened = false;
    $scope.statType = STAT_TYPE_AVG;
    $scope.phase = 1;

    $scope.openStartDate = function ()
    {
        $timeout(function ()
        {
            $scope.startDateOpened = true;
        });
    };

    $scope.selectPhase = function (phase)
    {
        $scope.phase = phase;
    };

    $scope.isPhase = function (phase)
    {
        return $scope.phase === phase;
    };

    $scope.selectStatTypeAVG = function ()
    {
        $scope.statType = STAT_TYPE_AVG;
    };

    $scope.selectStatTypeMIN = function ()
    {
        $scope.statType = STAT_TYPE_MIN;
    };

    $scope.selectStatTypeMAX = function ()
    {
        $scope.statType = STAT_TYPE_MAX;
    };

    $scope.isStatTypeAVG = function ()
    {
        return $scope.statType === STAT_TYPE_AVG;
    };

    $scope.isStatTypeMAX = function ()
    {
        return $scope.statType === STAT_TYPE_MAX;
    };

    $scope.isStatTypeMIN = function ()
    {
        return $scope.statType === STAT_TYPE_MIN;
    };

    $scope.openEndDate = function ()
    {
        $timeout(function ()
        {
            $scope.endDateOpened = true;
        });
    };

    var getStatValue = function (prefix)
    {
        if (undefined == $scope.stats) {
            return undefined;
        } else {
            var phaseStat = $scope.stats[prefix + $scope.phase];
            return undefined == phaseStat ? undefined : phaseStat[$scope.statType];
        }
    };

    $scope.getVoltage = function ()
    {
        return getStatValue("voltage");
    };

    $scope.getCurrent = function ()
    {
        return getStatValue("current");
    };

    $scope.getActivePower = function ()
    {
        return getStatValue("activePower");
    };

    $scope.getReactivePower = function ()
    {
        return getStatValue("reactivePower");
    };

    $scope.getApparentPower = function ()
    {
        return getStatValue("apparentPower");
    };

    $scope.getPowerFactor = function ()
    {
        return getStatValue("powerFactor");
    };

    $scope.getFrequency = function ()
    {
        return getStatValue("frequency");
    };

    function refreshStats()
    {
        DeviceDAO.getStats($routeParams.deviceId, $scope.startDate, $scope.endDate, function (stats)
        {
            $scope.stats = stats;
        });
    }

    refreshStats();

    $scope.$watch("startDate", refreshStats);
    $scope.$watch("endDate", refreshStats);
});
