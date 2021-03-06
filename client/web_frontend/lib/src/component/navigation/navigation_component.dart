import 'package:angular/angular.dart';
import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/basic/account.dart';
import 'package:demoweb_app/src/service/authorization_service_interface.dart';
import 'package:demoweb_app/src/basic/context.dart';
import 'package:demoweb_app/src/routes/route_paths.dart';
import 'package:demoweb_app/src/routes/routes.dart';
import 'package:demoweb_app/src/service/user_service_interface.dart';

@Component(
  selector: "navigation-sidebar",
  templateUrl: "navigation_component.html",
  styleUrls: ["navigation_component.css"],
  directives: [coreDirectives, routerDirectives],
  exports: [RoutePaths, Routes],
)
class NavigationComponent {
  bool expandAccountMenu = false;

  final UserServiceInterface _user_service;
  final AuthorizationServiceInterface _auth_service;

  NavigationComponent(this._user_service, this._auth_service);

  bool displayAccountlessUi() {
    return !identityStorage.hasIdentity();
  }

  void onClickSyncAccount() {}

  void onClickAccount() {
    expandAccountMenu = !expandAccountMenu;
  }

  void onClickSignUp() {
    signUp(null, _user_service, _auth_service);
  }
}
