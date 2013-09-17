var electrical = angular.module("Electrical",
        ['ui.bootstrap', 'ngResource', 'ngCookies', 'models.MessageFactory', 'http-auth-interceptor', 'services.Authenticator', 'services.ExceptionHandler',
            'services.i18n', 'services.UserDAO', 'directives.ngAuthentication', 'directives.ngMessages', 'directives.ngMsg', 'devices', 'users']);

electrical.config(['$routeProvider', '$locationProvider', function ($routeProvider, $locationProvider)
{
//    $locationProvider.html5Mode(true);
    $locationProvider.hashPrefix('!');
    $routeProvider.when('/error', {templateUrl: 'app/error.html'}).when('/me',
            {controller: 'UserEditCtrl', templateUrl: 'app/users/me.html'}).otherwise({redirectTo: '/devices'});

}]);

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

electrical.controller("LoginCtrl", function ($scope, $location, $route, authService, i18n, Authenticator, MessageFactory, UserDAO)
{
    var signupMode = false;
    $scope.credentials = {email: null, password: null};
    $scope.passwordConfirmation = "";

    $scope.login = function ()
    {
        Authenticator.login($scope.credentials.email, $scope.credentials.password, function ()
        {
            authService.loginConfirmed();
            $scope.credentials = {};
        });
    };
    $scope.logout = function ()
    {
        Authenticator.logout();
        $route.reload();
    };

    $scope.register = function ()
    {
        if ($scope.credentials.password != $scope.passwordConfirmation) {
            throw new Error("Passwords do not match!");
        }
        UserDAO.register($scope.credentials.email, $scope.credentials.password, function ()
        {
            $scope.showSigninForm();
            $scope.login();
            $location.path("/projects");
        }, function (data)
        {
            if ("string" === typeof data) {
                MessageFactory.error(data);
            } else {
                MessageFactory.error("Registration failed");
                var violations = data['constraintViolations'];
                if (undefined != violations) {
                    for (var i = 0; i < data['constraintViolations'].length; i++) {
                        MessageFactory.error(violations[i]['propertyPath'] + ":" + violations[i]['message']);
                    }
                }
            }
        });
    };

    $scope.showSignupForm = function ()
    {
        signupMode = true;
    };

    $scope.showSigninForm = function ()
    {
        signupMode = false;
    };

    $scope.isSignupMode = function ()
    {
        return signupMode;
    }
});

angular.isUndefinedOrNull = function (val)
{
    return angular.isUndefined(val) || val === null
};
