#include <Arduino.h>
#include <Wire.h>
#include <micro_ros_platformio.h>
#include <../extra_packages/example_interfaces/srv/add_two_ints.h>
#include <stdio.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int64.h>

rcl_node_t node;
rclc_support_t support;
rcl_allocator_t allocator;
rclc_executor_t executor;

rcl_service_t service;
rcl_wait_set_t wait_set;

example_interfaces__srv__AddTwoInts_Response res;
example_interfaces__srv__AddTwoInts_Request req;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){while(1){};}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

void service_callback(const void * req, void * res){
  example_interfaces__srv__AddTwoInts_Request * req_in = (example_interfaces__srv__AddTwoInts_Request *) req;
  example_interfaces__srv__AddTwoInts_Response * res_in = (example_interfaces__srv__AddTwoInts_Response *) res;

  //printf("Service request value: %d + %d.\n", (int) req_in->a, (int) req_in->b);

  res_in->sum = req_in->a + req_in->b;
}

void setup() {
  //set_microros_transports();
  set_microros_serial_transports(Serial);
  delay(1000); 

  allocator = rcl_get_default_allocator();

  // create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "add_twoints_client_rclc", "", &support));

  // create service
  RCCHECK(rclc_service_init_default(&service, &node, ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces, srv, AddTwoInts), "/addtwoints"));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_service(&executor, &service, &req, &res, service_callback));
}


void loop() {
  delay(100);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));}





// ============================================================================
// WORKING PUBLISHER AND SUBSCRIBER IN SINGLE NODE
// ============================================================================
// #include <Arduino.h>
// #include <Wire.h>
// #include <micro_ros_platformio.h>
// #include <rcl/rcl.h>
// #include <rclc/rclc.h>
// #include <rclc/executor.h>
// #include <std_msgs/msg/int32.h>

// #if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
// #error This example is only avaliable for Arduino framework with serial transport.
// #endif

// // debug
// #ifndef DebugMonitor
// #define DebugMonitor Serial6
// #endif

// #define debug(x) DebugMonitor.print(x)
// #define debugln(x) DebugMonitor.println(x)

// //publisher
// rcl_publisher_t publisher;
// std_msgs__msg__Int32 msg_pub;

// // subscriber
// rcl_subscription_t subscriber;
// std_msgs__msg__Int32 msg_sub;

// // publisher and subscriber common
// rcl_node_t node;
// rclc_support_t support;
// rcl_allocator_t allocator;
// rclc_executor_t executor;
// rcl_timer_t timer;
// unsigned int num_handles = 1 + 1;   // 1 subscriber, 1 publisher

// #define LED_PIN 13

// #define RCCHECK(fn){rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){debugln("error: ");debugln(temp_rc);debugln(RCL_RET_OK);error_loop();}}
// #define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// // Error handle loop
// void error_loop() {
//   while(1) {
//     digitalWrite(LED_PIN, !digitalRead(LED_PIN));
//     delay(500);
//   }
// }

// void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
//   RCLC_UNUSED(last_call_time);
//   if (timer != NULL) {
//     RCSOFTCHECK(rcl_publish(&publisher, &msg_pub, NULL));
//     msg_pub.data++;
//   }
// }

// void subscription_callback(const void * msgin)
// {  
//   const std_msgs__msg__Int32 * msg = (const std_msgs__msg__Int32 *)msgin;
//   digitalWrite(LED_PIN, (msg->data == 0) ? LOW : HIGH);  
//   debugln("=================================");
//   debugln(" RECEIVED:");
//   debugln("=================================");
//   debugln(msg->data);
  
// }

// void setup() {
//   // Configure serial transport
//   Serial.begin(115200);
//   set_microros_serial_transports(Serial);

//   // Debug
//   DebugMonitor.begin(115200);

//   pinMode(LED_PIN, OUTPUT);
//   digitalWrite(LED_PIN, HIGH); 

//   delay(2000);

//   allocator = rcl_get_default_allocator();

//   //create init_options
//   RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

//   // create node
//   RCCHECK(rclc_node_init_default(&node, "uros_platformio_node", "", &support));

//   // create publisher
//   RCCHECK(rclc_publisher_init_default(
//     &publisher,
//     &node,
//     ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
//     "micro_ros_platformio_node_publisher"));
  
//   // create subscriber
//   RCCHECK(rclc_subscription_init_default(
//     &subscriber,
//     &node,
//     ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
//     "micro_ros_platformio_node_subscriber"));

//   // create timer,
//   const unsigned int timer_timeout = 1000;
//   RCCHECK(rclc_timer_init_default(
//     &timer,
//     &support,
//     RCL_MS_TO_NS(timer_timeout),
//     timer_callback));

//   // create executor
//   RCCHECK(rclc_executor_init(&executor, &support.context, num_handles, &allocator));  
//   RCCHECK(rclc_executor_add_timer(&executor, &timer));
//   RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg_sub, &subscription_callback, ON_NEW_DATA));   

//   msg_pub.data = 0;
// }

// void loop() {
//   delay(100);
//   RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
// }





















































// """
// PlatformIO workflow:

// pio lib install # Install dependencies
// pio run # Build the firmware
// pio run --target upload # Flash the firmware


// After the library is compiled for first time the build process
// will be skipped, to trigger a library build and apply library
// modifications on your next platformIO build:

// pio run --target clean_microros  # Clean library
// """
// // ----------------------------------------------------------------------------
// // ORIGINAL PUBLISHER CODE THAT WORKS
// // ----------------------------------------------------------------------------
// #include <Arduino.h>
// #include <micro_ros_platformio.h>
// #include <rcl/rcl.h>
// #include <rclc/rclc.h>
// #include <rclc/executor.h>
// #include <std_msgs/msg/int32.h>
// #include <std_msgs/msg/int64.h>
// //#include <example_interfaces/srv/add_two_ints.h>

// #if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
// #error This example is only avaliable for Arduino framework with serial transport.
// #endif

// // Publisher object
// rcl_publisher_t publisher;
// std_msgs__msg__Int32 msg;

// // Service server object
// // rcl_service_t service;
// // const char * service_name = "/addtwoints";

// // define rclc executor
// rclc_executor_t executor;

// // support object
// rclc_support_t support;

// // micro-ROS allocator object
// rcl_allocator_t allocator;

// // create node object
// rcl_node_t node;
// // Create and initialize timer object
// rcl_timer_t timer;

// #define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
// #define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// // Error handle loop
// void error_loop() {
//   while(1) {
//     delay(100);
//   }
// }

// void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
//   RCLC_UNUSED(last_call_time);
//   if (timer != NULL) {
//     RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
//     msg.data++;
//   }
// }

// void setup() {
//   // Configure serial transport
//   Serial.begin(115200);
//   set_microros_serial_transports(Serial);
//   delay(2000);

//   // Initialize micro-ROS allocator
//   allocator = rcl_get_default_allocator();

//   //create init_options
//   RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

//   // create node
//   RCCHECK(rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));

//   // create publisher
//   RCCHECK(rclc_publisher_init_default(
//     &publisher,
//     &node,
//     ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
//     "micro_ros_platformio_node_publisher"));

//   // create service
//   // Get message type support
//   // const rosidl_message_type_support_t * type_support = ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces, srv, AddTwoInts);

//   // create timer,
//   const unsigned int timer_timeout = 1000;

//   // Initialize support object
//   RCCHECK(rclc_timer_init_default(
//     &timer,
//     &support,
//     RCL_MS_TO_NS(timer_timeout),
//     timer_callback));

//   // create executor
//   RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
//   RCCHECK(rclc_executor_add_timer(&executor, &timer));

//   msg.data = 0;
// }

// void loop() {
//   delay(100);
//   RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
// }



// //#include <micro_ros_arduino.h>
// #include <Arduino.h>
// #include <micro_ros_platformio.h>
// #include <example_interfaces/srv/add_two_ints.h>
// #include <stdio.h>
// #include <rcl/error_handling.h>
// #include <rclc/rclc.h>
// #include <rclc/executor.h>
// #include <std_msgs/msg/int64.h>

// rcl_node_t node;
// rclc_support_t support;
// rcl_allocator_t allocator;
// rclc_executor_t executor;

// rcl_service_t service;
// rcl_wait_set_t wait_set;

// example_interfaces__srv__AddTwoInts_Response res;
// example_interfaces__srv__AddTwoInts_Request req;

// #define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){while(1){};}}
// #define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// void service_callback(const void * req, void * res){
//   example_interfaces__srv__AddTwoInts_Request * req_in = (example_interfaces__srv__AddTwoInts_Request *) req;
//   example_interfaces__srv__AddTwoInts_Response * res_in = (example_interfaces__srv__AddTwoInts_Response *) res;

//   //printf("Service request value: %d + %d.\n", (int) req_in->a, (int) req_in->b);

//   res_in->sum = req_in->a + req_in->b;
// }

// void setup() {
//   //set_microros_transports();
//   Serial.begin(115200);
//   set_microros_serial_transports(Serial);
//   delay(1000); 

//   allocator = rcl_get_default_allocator();

//   // create init_options
//   RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

//   // create node
//   RCCHECK(rclc_node_init_default(&node, "add_twoints_client_rclc", "", &support));

//   // create service
//   RCCHECK(rclc_service_init_default(&service, &node, ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces, srv, AddTwoInts), "/addtwoints"));

//   // create executor
//   RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
//   RCCHECK(rclc_executor_add_service(&executor, &service, &req, &res, service_callback));
// }


// void loop() {
//   delay(100);
//   RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
// }































// // ----------------------------------------------------------------------------
// // ORIGINAL PUBLISHER CODE THAT WORKS
// // ----------------------------------------------------------------------------
// // #include <Arduino.h>
// // #include <micro_ros_platformio.h>

// // #include <rcl/rcl.h>
// // #include <rclc/rclc.h>
// // #include <rclc/executor.h>

// // #include <std_msgs/msg/int32.h>

// // #if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
// // #error This example is only avaliable for Arduino framework with serial transport.
// // #endif

// // rcl_publisher_t publisher;
// // std_msgs__msg__Int32 msg;

// // rclc_executor_t executor;
// // rclc_support_t support;
// // rcl_allocator_t allocator;
// // rcl_node_t node;
// // rcl_timer_t timer;

// // #define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
// // #define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// // // Error handle loop
// // void error_loop() {
// //   while(1) {
// //     delay(100);
// //   }
// // }

// // void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
// //   RCLC_UNUSED(last_call_time);
// //   if (timer != NULL) {
// //     RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
// //     msg.data++;
// //   }
// // }

// // void setup() {
// //   // Configure serial transport
// //   Serial.begin(115200);
// //   set_microros_serial_transports(Serial);
// //   delay(2000);

// //   allocator = rcl_get_default_allocator();

// //   //create init_options
// //   RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

// //   // create node
// //   RCCHECK(rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));

// //   // create publisher
// //   RCCHECK(rclc_publisher_init_default(
// //     &publisher,
// //     &node,
// //     ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
// //     "micro_ros_platformio_node_publisher"));

// //   // create timer,
// //   const unsigned int timer_timeout = 1000;
// //   RCCHECK(rclc_timer_init_default(
// //     &timer,
// //     &support,
// //     RCL_MS_TO_NS(timer_timeout),
// //     timer_callback));

// //   // create executor
// //   RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
// //   RCCHECK(rclc_executor_add_timer(&executor, &timer));

// //   msg.data = 0;
// // }

// // void loop() {
// //   delay(100);
// //   RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
// // }