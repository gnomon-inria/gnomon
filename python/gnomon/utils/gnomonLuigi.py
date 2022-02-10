import luigi

import gnomon.core


class DynamicFormTarget(luigi.Target):
    def __init__(self, form):
        super().__init__()
        self.form = form

    def exists(self):
        return isinstance(self.form, gnomon.core.gnomonAbstractDynamicForm) and len(self.form.times()) > 0


class AlgorithmPluginTask(luigi.Task):
    task_name = luigi.Parameter()
    plugin_name = luigi.Parameter()

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.input_names = []
        self.input_tasks = {}
        self.input_output_names = {}

        self.form_output_functions = {}
        self.file_output_paths = {}

    def requires(self):
        for input_name in self.input_names:
            if input_name in self.input_tasks.keys():
                yield self.input_tasks[input_name]

    def output(self):
        outputs = {}
        for output_name in self.form_output_functions.keys():
            outputs[output_name] = DynamicFormTarget(self.form_output_functions[output_name]())
        for output_name in self.file_output_paths.keys():
            outputs[output_name] = luigi.LocalTarget(self.file_output_paths[output_name])
        return outputs

    def connect_input(self, other_task, output_name=None, input_name=None):
        if input_name is None:
            input_name = self.input_names[len(self.input_tasks)]
        self.input_tasks[input_name] = other_task
        if output_name is None:
            if len(other_task.form_output_functions) > 0:
                output_name = list(other_task.form_output_functions.keys())[0]
            elif len(other_task.file_output_paths) > 0:
                output_name = list(other_task.file_output_paths.keys())[0]
        self.input_output_names[input_name] = output_name

    def algorithm_inputs(self):
        inputs = {}
        input_targets = super().input()
        i_task = 0
        for input_name in self.input_names:
            if input_name in self.input_tasks.keys():
                task_targets = input_targets[i_task]
                print(task_targets)
                output_name = self.input_output_names[input_name]
                if isinstance(task_targets[output_name] ,DynamicFormTarget):
                    inputs[input_name] = task_targets[output_name].form
                    i_task += 1
                else:
                    inputs[input_name] = None
            else:
                inputs[input_name] = None
        return inputs
