from typing import Tuple
from typing import List
import math
import tensorflow as tf
import numpy as np

from common_module import PRelu
from common_module import ShlFeaturePlanesBuilder

INPUT_SIZE = 11

class LinearCnnLayer(tf.Module):
    def __init__(self, num_features: int):
        num_input_channels = num_features
        num_filters = num_features
        kernel_size = 1
        self.conv_kernel = tf.Variable(
            name="linear_conv_kernel",
            initial_value=tf.random.truncated_normal( # pylint: disable=unexpected-keyword-arg
                shape=[kernel_size, kernel_size, 
                       num_input_channels, 
                       num_filters],
                stddev=math.sqrt(2/(kernel_size*kernel_size*num_input_channels)),
                dtype=tf.float32))
    
    @tf.function
    def __call__(self, feature_planes: tf.Tensor):
        return tf.nn.conv2d(
            input=feature_planes,
            filters=self.conv_kernel,
            strides=[1, 1, 1, 1],
            padding="SAME",
            name="linear_conv_kernel")

    @tf.function
    def TransformVariables(self) -> List[tf.Tensor]:
        return [self.conv_kernel]

class CnnLayers(tf.Module):
    def __init__(self, num_features: int):
        num_input_channels = num_features
        num_filters = 22
        kernel_size = 5
        self.conv_kernel1 = tf.Variable(
            name="conv_kernel1",
            initial_value=tf.random.truncated_normal( # pylint: disable=unexpected-keyword-arg
                shape=[kernel_size, kernel_size, 
                       num_input_channels, 
                       num_filters],
                stddev=math.sqrt(2/(kernel_size*kernel_size*num_input_channels)),
                dtype=tf.float32))
        self.conv_biases1 = tf.Variable(
            name="conv_biases1",
            initial_value=tf.zeros(shape=[num_filters], dtype=tf.float32))
        self.prelu1 = PRelu(num_features=num_filters)
        
        kernel_size = 3
        num_input_channels = num_filters
        num_filters = 22
        self.conv_kernel2 = tf.Variable(
            name="conv_kernel2",
            initial_value=tf.random.truncated_normal( # pylint: disable=unexpected-keyword-arg
                shape=[kernel_size, kernel_size, 
                       num_input_channels, 
                       num_filters],
                stddev=math.sqrt(2/(kernel_size*kernel_size*num_input_channels)),
                dtype=tf.float32))
        self.conv_biases2 = tf.Variable(
            name="conv_biases2",
            initial_value=tf.zeros(shape=[num_filters], dtype=tf.float32))
        self.prelu2 = PRelu(num_features=num_filters)

        kernel_size = 1
        num_input_channels = num_filters
        num_filters = 22
        self.conv_kernel3 = tf.Variable(
            name="conv_kernel3",
            initial_value=tf.random.truncated_normal( # pylint: disable=unexpected-keyword-arg
                shape=[kernel_size, kernel_size, 
                       num_input_channels, 
                       num_filters],
                stddev=math.sqrt(2/(kernel_size*kernel_size*num_input_channels)),
                dtype=tf.float32))
        self.conv_biases3 = tf.Variable(
            name="conv_biases3",
            initial_value=tf.zeros(shape=[num_filters], dtype=tf.float32))
        self.prelu3 = PRelu(num_features=num_filters)

    @tf.function
    def __call__(self, feature_planes: tf.Tensor):
        linear1 = tf.nn.conv2d(
            input=feature_planes,
            filters=self.conv_kernel1,
            strides=[1, 1, 1, 1],
            padding="SAME",
            name="conv1") + self.conv_biases1
        output1 = self.prelu1(linear1)

        linear2 = tf.nn.conv2d(
            input=output1,
            filters=self.conv_kernel2,
            strides=[1, 1, 1, 1],
            padding="SAME",
            name="conv2") + self.conv_biases2
        output2 = self.prelu2(linear2)

        linear3 = tf.nn.conv2d(
            input=output2,
            filters=self.conv_kernel3,
            strides=[1, 1, 1, 1],
            padding="SAME",
            name="conv3") + self.conv_biases3
        output3 = self.prelu3(linear3)

        return output3
    
    @tf.function
    def TransformVariables(self) -> List[tf.Tensor]:
        return [self.conv_kernel1, self.conv_kernel2, self.conv_kernel3]

class PolicyLayer(tf.Module):
    def __init__(self, num_features: int):
        num_input_channels = num_features
        num_filters = 1
        kernel_size = 1
        self.conv_kernel = tf.Variable(
            name="policy_conv_kernel",
            initial_value=tf.random.truncated_normal( # pylint: disable=unexpected-keyword-arg
                shape=[kernel_size, kernel_size, 
                       num_input_channels, 
                       num_filters],
                stddev=math.sqrt(2/(kernel_size*kernel_size*num_input_channels)),
                dtype=tf.float32))
        self.conv_biases = tf.Variable(
            name="policy_conv_biases",
            initial_value=tf.zeros(shape=[num_filters], dtype=tf.float32))
        self.prelu = PRelu(num_features=num_filters)

        num_inputs = INPUT_SIZE*INPUT_SIZE*num_filters
        num_outputs = INPUT_SIZE*INPUT_SIZE + 5
        self.weights = tf.Variable(
            name="policy_weights",
            initial_value=tf.random.truncated_normal( # pylint: disable=unexpected-keyword-arg
                shape=[num_inputs, num_outputs],
                stddev=math.sqrt(2/(num_inputs)),
                dtype=tf.float32))
        self.biases = tf.Variable(
            name="policy_biases",
            initial_value=tf.zeros(shape=[num_outputs], dtype=tf.float32))

    @tf.function
    def __call__(self, conv_features: tf.Tensor) -> [tf.Tensor, tf.Tensor]:
        fconv_linear = tf.nn.conv2d(
            input=conv_features,
            filters=self.conv_kernel,
            strides=[1, 1, 1, 1],
            padding="SAME",
            name="policy_fconv") + self.conv_biases
        fconv_output = self.prelu(fconv_linear)

        flattened_features = tf.reshape(
            tensor=fconv_output,
            shape=[-1, INPUT_SIZE*INPUT_SIZE],
            name="flattened_policy_features")

        logits = tf.matmul(
            a=flattened_features,
            b=self.weights,
            name="policy_dense") + self.biases
        policy = tf.nn.softmax(
            logits=logits, name="policy")
        
        return policy, logits
    
    @tf.function
    def TransformVariables(self) -> List[tf.Tensor]:
        return [self.conv_kernel, self.weights]

class ValueLayer(tf.Module):
    def __init__(self, num_features: int):
        num_input_channels = num_features
        num_filters = 2
        kernel_size = 1
        self.conv_kernel = tf.Variable(
            name="value_conv_kernel",
            initial_value=tf.random.truncated_normal( # pylint: disable=unexpected-keyword-arg
                shape=[kernel_size, kernel_size, 
                       num_input_channels, 
                       num_filters],
                stddev=math.sqrt(2/(kernel_size*kernel_size*num_input_channels)),
                dtype=tf.float32))
        self.conv_biases = tf.Variable(
            name="value_conv_biases",
            initial_value=tf.zeros(shape=[num_filters], dtype=tf.float32))
        self.prelu1 = PRelu(num_features=num_filters)

        num_inputs = INPUT_SIZE*INPUT_SIZE*num_filters
        num_outputs = 49
        self.weights1 = tf.Variable(
            name="value_weights1",
            initial_value=tf.random.truncated_normal( # pylint: disable=unexpected-keyword-arg
                shape=[num_inputs, num_outputs],
                stddev=math.sqrt(2/(num_inputs)),
                dtype=tf.float32))
        self.biases1 = tf.Variable(
            name="policy_biases1",
            initial_value=tf.zeros(shape=[num_outputs], dtype=tf.float32))
        self.prelu2 = PRelu(num_features=num_outputs)

        num_inputs = num_outputs
        num_outputs = 1
        self.weights2 = tf.Variable(
            name="value_weights2",
            initial_value=tf.random.truncated_normal( # pylint: disable=unexpected-keyword-arg
                shape=[num_inputs, num_outputs],
                stddev=math.sqrt(2/(num_inputs)),
                dtype=tf.float32))
        self.biases2 = tf.Variable(
            name="policy_biases2",
            initial_value=tf.zeros(shape=[num_outputs], dtype=tf.float32))

    @tf.function
    def __call__(self, conv_features: tf.Tensor) -> tf.Tensor:
        value_summary_linear = tf.nn.conv2d(
            input=conv_features,
            filters=self.conv_kernel,
            strides=[1, 1, 1, 1],
            padding="SAME",
            name="value_fconv") + self.conv_biases
        value_summary = self.prelu1(value_summary_linear)

        flattened_value_summary = tf.reshape(
            tensor=value_summary,
            shape=[-1, INPUT_SIZE*INPUT_SIZE*2],
            name="flattened_value_features")
        
        value_scores_linear = tf.matmul(
            a=flattened_value_summary,
            b=self.weights1,
            name="value_dense1") + self.biases1
        value_scores = self.prelu2(value_scores_linear)

        value_score = tf.matmul(
            a=value_scores,
            b=self.weights2,
            name="value_dense2") + self.biases2
        value = tf.tanh(x=value_score, name="value")

        return tf.reshape(tensor=value, shape=[-1])

    @tf.function
    def TransformVariables(self) -> List[tf.Tensor]:
        return [self.conv_kernel, self.weights1, self.weights2]

class GomokuCnnShlSharedModel(tf.Module):
    def __init__(self):
        self.feature_planes_builder = ShlFeaturePlanesBuilder(
            input_size=INPUT_SIZE)
        self.linear_cnn_layer = LinearCnnLayer(num_features=2 + 5 + 3 + 4)
        self.cnn_layers = CnnLayers(num_features=2 + 5 + 3 + 4)
        self.policy_layer = PolicyLayer(num_features=22)
        self.value_layer = ValueLayer(num_features=22)
    
    @tf.function(
        input_signature=[
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.uint8),
            tf.TensorSpec(shape=[None], dtype=tf.uint8),
            tf.TensorSpec(shape=[None], dtype=tf.uint8),
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.float32),
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.float32),
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.float32),
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.float32)
        ])
    def Infer(self, 
              boards: tf.Tensor,
              game_phases: tf.Tensor,
              next_move_stone_types: tf.Tensor,
              primary_shl_count_black: tf.Tensor,
              secondary_shl_count_black: tf.Tensor,
              primary_shl_count_white: tf.Tensor,
              secondary_shl_count_white: tf.Tensor) -> Tuple[tf.Tensor, tf.Tensor, tf.Tensor]:
        feature_planes = self.feature_planes_builder(
            boards,
            game_phases,
            next_move_stone_types,
            primary_shl_count_black,
            secondary_shl_count_black,
            primary_shl_count_white,
            secondary_shl_count_white)
        
        conv_features = self.cnn_layers(feature_planes)

        policy, policy_logits = self.policy_layer(conv_features)
        value = self.value_layer(conv_features)

        return policy, value, policy_logits

    @tf.function(
        input_signature=[
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.uint8),
            tf.TensorSpec(shape=[None], dtype=tf.uint8),
            tf.TensorSpec(shape=[None], dtype=tf.uint8),
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.float32),
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.float32),
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.float32),
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.float32)
        ])
    def __call__(self,
                 boards: tf.Tensor,
                 game_phases: tf.Tensor,
                 next_move_stone_types: tf.Tensor,
                 primary_shl_count_black: tf.Tensor,
                 secondary_shl_count_black: tf.Tensor,
                 primary_shl_count_white: tf.Tensor,
                 secondary_shl_count_white: tf.Tensor) -> Tuple[tf.Tensor, tf.Tensor]:
        policy, value, _ = self.Infer(
            boards, game_phases, next_move_stone_types,
            primary_shl_count_black, secondary_shl_count_black,
            primary_shl_count_white, secondary_shl_count_white)
        return policy, value

    @tf.function
    def TransformVariables(self) -> List[tf.Tensor]:
        return self.cnn_layers.TransformVariables() + \
               self.policy_layer.TransformVariables() + \
               self.value_layer.TransformVariables()

    @tf.function(
        input_signature=[
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.uint8),
            tf.TensorSpec(shape=[None], dtype=tf.uint8),
            tf.TensorSpec(shape=[None], dtype=tf.uint8),
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.float32),
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.float32),
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.float32),
            tf.TensorSpec(shape=[None, INPUT_SIZE, INPUT_SIZE], dtype=tf.float32),
            tf.TensorSpec(shape=[None, INPUT_SIZE*INPUT_SIZE + 5], dtype=tf.float32),
            tf.TensorSpec(shape=[None], dtype=tf.float32)
        ])
    def Loss(self,
             boards: tf.Tensor,
             game_phases: tf.Tensor,
             next_move_stone_types: tf.Tensor,
             primary_shl_count_black: tf.Tensor,
             secondary_shl_count_black: tf.Tensor,
             primary_shl_count_white: tf.Tensor,
             secondary_shl_count_white: tf.Tensor,
             policies: tf.Tensor,
             values: tf.Tensor) -> Tuple[tf.Tensor,
                                         tf.Tensor,
                                         tf.Tensor,
                                         tf.Tensor]:
        _, pred_values, policy_logits = self.Infer(
            boards, game_phases, next_move_stone_types,
            primary_shl_count_black, secondary_shl_count_black,
            primary_shl_count_white, secondary_shl_count_white)

        # Policy loss.
        policy_losses = tf.nn.softmax_cross_entropy_with_logits(
            labels=policies, logits=policy_logits, axis=1)
        policy_loss = tf.reduce_mean(
            input_tensor=policy_losses, axis=0)

        # Value loss.
        value_loss = tf.losses.mean_squared_error(
            y_true=values, y_pred=pred_values)
        
        # Regularization loss.
        l1_loss = 0
        for transform_var in self.TransformVariables():
            flattened = tf.reshape(tensor=transform_var, shape=[-1])
            l1 = tf.reduce_sum(tf.abs(x=flattened))
            l1_loss += l1
        l1_loss *= 1e-5

        loss = policy_loss + value_loss + l1_loss

        return loss, policy_loss, value_loss, l1_loss

    @tf.function(input_signature=[])
    def Name(self) -> tf.Tensor:
        return tf.constant(
            value="gomoku_cnn_shl_shared_i{0}".format(INPUT_SIZE),
            dtype=tf.string)

def TrainableVariables(model: GomokuCnnShlSharedModel):
    return [model.linear_cnn_layer.conv_kernel,
            model.cnn_layers.conv_kernel1,
            model.cnn_layers.conv_biases1,
            model.cnn_layers.conv_kernel2,
            model.cnn_layers.conv_biases2,
            model.cnn_layers.conv_kernel3,
            model.cnn_layers.conv_biases3,
            model.cnn_layers.prelu1.alphas,
            model.cnn_layers.prelu2.alphas,
            model.cnn_layers.prelu3.alphas,
            model.policy_layer.conv_kernel,
            model.policy_layer.conv_biases,
            model.policy_layer.prelu.alphas,
            model.policy_layer.weights,
            model.policy_layer.biases,
            model.value_layer.conv_kernel,
            model.value_layer.conv_biases,
            model.value_layer.prelu1.alphas,
            model.value_layer.prelu2.alphas,
            model.value_layer.weights1,
            model.value_layer.biases1,
            model.value_layer.weights2,
            model.value_layer.biases2]

def PrintModelParameterInfo():
    model = GomokuCnnShlSharedModel()

    num_params = 0
    for variable in model.variables:
        dims = 1
        for dim in variable.shape:
            dims *= dim
        
        print(variable.name, "\t", dims)

        num_params += dims
    
    print("num_params=", num_params)

if __name__ == "__main__":
    PrintModelParameterInfo()
