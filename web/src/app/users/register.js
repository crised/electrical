users.controller('UserRegistrationCtrl', function ($scope, $location, MessageFactory, UserDAO)
{
    $scope.user = {id: null};

    $scope.register = function ()
    {
        UserDAO.persistUser(this.user, function (user)
        {
            MessageFactory.info("User saved successfully");
            $scope.user = user;
            originalUser = angular.extend({}, $scope.user);
        });
    };

});
