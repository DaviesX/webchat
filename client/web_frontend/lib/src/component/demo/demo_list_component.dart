import 'package:angular/angular.dart';
import 'package:demoweb_app/src/component/footer/footer_component.dart';

@Component(
  selector: "demo-list",
  templateUrl: "demo_list_component.html",
  directives: [coreDirectives, FooterComponent],
)
class DemoListComponent {}
