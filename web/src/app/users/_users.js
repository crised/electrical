var users = angular.module('users', ['services.UserDAO', 'directives.ngFocus']);

devices.config(['$routeProvider', function ($routeProvider)
{
    $routeProvider.when('/me', {controller: 'UserEditCtrl', templateUrl: 'app/users/me.html'}).when('/users',
            {controller: 'UserListCtrl', templateUrl: 'app/users/userList.html'}).when('/users/register',
            {controller: 'UserRegistrationCtrl', templateUrl: 'app/users/register.html'}).when('/users/:userId',
            {controller: 'UserEditCtrl', templateUrl: 'app/users/userDetails.html'});
}]);
