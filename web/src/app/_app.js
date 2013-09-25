var electrical = angular.module("Electrical", ['ui.bootstrap', 'ngResource', 'models.MessageFactory', 'services.ExceptionHandler', 'devices', 'users']);

electrical.config(['$routeProvider', '$locationProvider', function ($routeProvider, $locationProvider)
{
//    $locationProvider.html5Mode(true);
    $locationProvider.hashPrefix('!');
    $routeProvider.when('/error', {templateUrl: 'app/error.html'}).when('/me',
            {controller: 'UserEditCtrl', templateUrl: 'app/users/me.html'}).otherwise({redirectTo: '/devices'});

}]);

electrical.controller("LogoutCtrl", function ($scope, $location, $timeout)
{

    $scope.logout = function ()
    {
        jQuery.ajax(applicationContextPath + "/rest/user/me", {username: "logout", error: function ()
        {
            $timeout(function ()
            {
                $scope.$root.errorMessage = "Invalid credentials!";
                $location.path("/error");
            })

        }});
    }
});

electrical.config(function ($provide)
{
    $provide.decorator("$exceptionHandler", function ($delegate, MessageFactory)
    {
        return function (exception, cause)
        {
            $delegate(exception, cause);
            MessageFactory.error(exception.message);
        };
    });
});

angular.isUndefinedOrNull = function (val)
{
    return angular.isUndefined(val) || val === null
};
