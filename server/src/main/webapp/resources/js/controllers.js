(function() {
	angular.module('app.controllers', []).controller(
			'MainController',
			function($scope, $rootScope, $http, i18n, $location) {
				$scope.language = function() {
					return i18n.language;
				};
				$scope.setLanguage = function(lang) {
					i18n.setLanguage(lang);
				};
				$scope.activeWhen = function(value) {
					return value ? 'active' : '';
				};

				$scope.path = function() {
					return $location.url();
				};

				$scope.login = function() {
					$scope.$emit('event:loginRequest', $scope.username,
							$scope.password);
					$('#login').modal('hide');
				};
				$scope.logout = function() {
					$rootScope.user = null;
					$scope.username = $scope.password = null;
					$scope.$emit('event:logoutRequest');
					$location.url('/person');
				};

			}).controller('MembersCtrl',
			function MembersCtrl($scope, $http, Members) {

				// Define a refresh function, that updates the data from the
				// REST service
				$scope.refresh = function() {
					$scope.members = Members.query();
				};

				// Define a reset function, that clears the prototype newMember
				// object, and
				// consequently, the form
				$scope.reset = function() {
					// clear input fields
					$scope.newMember = {};
				};

				// Define a register function, which adds the member using the
				// REST service,
				// and displays any error messages
				$scope.register = function() {
					$scope.successMessages = '';
					$scope.errorMessages = '';
					$scope.errors = {};

					Members.save($scope.newMember, function(data) {

						// mark success on the registration form
						$scope.successMessages = [ 'Member Registered' ];

						// Update the list of members
						$scope.refresh();

						// Clear the form
						$scope.reset();
					}, function(result) {
						if ((result.status == 409) || (result.status == 400)) {
							$scope.errors = result.data;
						} else {
							$scope.errorMessages = [ 'Unknown  server error' ];
						}
						$scope.$apply();
					});

				};

				// Call the refresh() function, to populate the list of members
				$scope.refresh();

				// Initialize newMember here to prevent Angular from sending a
				// request
				// without a proper Content-Type.
				$scope.reset();

				// Set the default orderBy to the name property
				$scope.orderBy = 'name';
			});
}());