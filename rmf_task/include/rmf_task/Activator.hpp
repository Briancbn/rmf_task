/*
 * Copyright (C) 2021 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef RMF_TASK__ACTIVATOR_HPP
#define RMF_TASK__ACTIVATOR_HPP

#include <rmf_task/Request.hpp>

namespace rmf_task {

//==============================================================================
/// A factory for generating Task::Active instances from requests.
class Activator
{
public:

  /// Construct an empty TaskFactory
  Activator();

  /// Signature for activating a task
  ///
  /// \tparam Description
  ///   A class that implements the Request::Description interface
  ///
  /// \param[in] request
  ///   An immutable reference to the relevant task request
  ///
  /// \param[in] description
  ///   The down-casted description of the task
  ///
  /// \param[in] backup_state
  ///   The serialized backup state of the Task, if the Task is being restored
  ///   from a crash or disconnection. If the Task is not being restored, a
  ///   std::nullopt will be passed in here.
  ///
  /// \param[in] update
  ///   A callback that will be triggered when the task has a significant
  ///   update in its status.
  ///
  /// \param[in] finished
  ///   A callback that will be triggered when the task has finished.
  ///
  /// \return an active, running instance of the requested task.
  template<typename Description>
  using Activate =
    std::function<
    Task::ActivePtr(
      const Task::ConstBookingPtr& request,
      const Description& description,
      std::optional<std::string> backup_state,
      std::function<void(Phase::ConstSnapshotPtr)> update,
      std::function<void(Phase::ConstCompletedPtr)> phase_finished,
      std::function<void()> task_finished)
    >;

  /// Add a callback to convert from a Request into an active Task.
  ///
  /// \tparam Description
  ///   A class that implements the Request::Description interface
  ///
  /// \param[in] activator
  ///   A callback that activates a Task matching the Description
  template<typename Description>
  void add_activator(Activate<Description> activator);

  /// Activate a Task object based on a Request::Description.
  ///
  /// \param[in] request
  ///   The task request
  ///
  /// \param[in] update
  ///   A callback that will be triggered when the task has a significant update
  ///
  /// \param[in] phase_finished
  ///   A callback that will be triggered whenever a task phase is finished
  ///
  /// \param[in] task_finished
  ///   A callback that will be triggered when the task has finished
  ///
  /// \return an active, running instance of the requested task.
  std::shared_ptr<Task> activate(
    const Request& request,
    std::function<void(Phase::ConstSnapshotPtr)> update,
    std::function<void(Phase::ConstCompletedPtr)> phase_finished,
    std::function<void()> task_finished);

  /// Restore a Task that crashed or disconnected.
  ///
  /// \param[in] request
  ///   The task request
  ///
  /// \param[in] backup_state
  ///   The serialized backup state of the Task
  ///
  /// \param[in] update
  ///   A callback that will be triggered when the task has a significant update
  ///
  /// \param[in] phase_finished
  ///   A callback that will be triggered whenever a task phase is finished
  ///
  /// \param[in] task_finished
  ///   A callback that will be triggered when the task has finished
  ///
  /// \return an active, running instance of the requested task.
  std::shared_ptr<Task> restore(
    const Request& request,
    std::string backup_state,
    std::function<void(Phase::ConstSnapshotPtr)> update,
    std::function<void(Phase::ConstCompletedPtr)> phase_finished,
    std::function<void()> task_finished);

  class Implementation;
private:
  rmf_utils::impl_ptr<Implementation> _pimpl;
};


} // namespace rmf_task


#endif // RMF_TASK__ACTIVATOR_HPP