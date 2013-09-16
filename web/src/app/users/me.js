users.controller('UserEditCtrl', function ($scope, $location, Authenticator, MessageFactory, UserDAO)
{
    var originalUser , editMode, viewReady;
    $scope.user = {id: null};
    $scope.password = {current: null, "new": null, confirmation: null};
    function clearPasswords()
    {
        $scope.password.current = "";
        $scope.password.new = "";
        $scope.password.confirmation = "";
    }

    clearPasswords();

    UserDAO.getMe(function (user)
    {
        if (originalUser == null && user != null) {
            originalUser = angular.extend({}, user);
        }
        $scope.user = user;
        viewReady = true;
    });

    $scope.isModified = function ()
    {
        return originalUser != null && this.user != null && (originalUser.firstName != this.user.firstName || originalUser.lastName != this.user.lastName);
    };
    $scope.isEditMode = function ()
    {
        return editMode;
    };
    $scope.isViewReady = function ()
    {
        return viewReady;
    };

    $scope.edit = function ()
    {
        editMode = true;
    };

    $scope.save = function ()
    {
        UserDAO.persistUser(this.user, function ()
        {
            MessageFactory.info("User saved successfully");
        });
    };

    $scope.changePassword = function ()
    {
        if ($scope.password.new != $scope.password.confirmation) {
            MessageFactory.warn("Passwords do not match");
            return;
        }
        UserDAO.changePassword($scope.password.current, $scope.password.new, function (token)
        {
            Authenticator.setToken(token);
            MessageFactory.info("Password changed successfully");
        }, function (response, status)
        {
            if (412 == status) {
                MessageFactory.error(response);
            } else {
                throw new Error(response);
            }
        });
        clearPasswords();
    };
});
