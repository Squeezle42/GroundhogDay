#!/usr/bin/env python3
'''
Context Manager for AI Collaboration

This script helps manage context when working with multiple AI threads.
It allows saving, loading, and generating context summaries for different
parts of your project.
'''

import os
import json
import time
import argparse
from datetime import datetime
import re

class AIContextManager:
    def __init__(self, project_root=None):
        self.project_root = project_root or os.getcwd()
        self.contexts_dir = os.path.join(self.project_root, '.ai_contexts')
        self.ensure_contexts_dir()
        
    def ensure_contexts_dir(self):
        """Create the contexts directory if it doesn't exist."""
        if not os.path.exists(self.contexts_dir):
            os.makedirs(self.contexts_dir)
            
    def create_context(self, name, description=None, domain=None, files=None):
        """Create a new context area."""
        context_path = os.path.join(self.contexts_dir, f"{name}.json")
        
        if os.path.exists(context_path):
            raise ValueError(f"Context '{name}' already exists.")
            
        context = {
            'name': name,
            'description': description or f"Context for {name}",
            'domain': domain or 'general',
            'created': datetime.now().isoformat(),
            'updated': datetime.now().isoformat(),
            'files': files or [],
            'key_points': [],
            'decisions': [],
            'history': []
        }
        
        with open(context_path, 'w') as f:
            json.dump(context, f, indent=2)
            
        print(f"Created new context: {name}")
        return context
        
    def get_context(self, name):
        """Load a context by name."""
        context_path = os.path.join(self.contexts_dir, f"{name}.json")
        
        if not os.path.exists(context_path):
            raise ValueError(f"Context '{name}' does not exist.")
            
        with open(context_path, 'r') as f:
            return json.load(f)
            
    def update_context(self, name, update_data):
        """Update an existing context."""
        context = self.get_context(name)
        
        for key, value in update_data.items():
            if key in context and isinstance(context[key], list) and isinstance(value, list):
                context[key].extend(value)
            else:
                context[key] = value
                
        context['updated'] = datetime.now().isoformat()
        
        context_path = os.path.join(self.contexts_dir, f"{name}.json")
        with open(context_path, 'w') as f:
            json.dump(context, f, indent=2)
            
        return context
        
    def add_key_point(self, context_name, key_point):
        """Add a key point to the context."""
        self.update_context(context_name, {
            'key_points': [key_point]
        })
        
    def add_decision(self, context_name, decision):
        """Add a decision to the context."""
        self.update_context(context_name, {
            'decisions': [{
                'date': datetime.now().isoformat(),
                'decision': decision
            }]
        })
        
    def add_history_item(self, context_name, prompt, response):
        """Add a conversation history item."""
        self.update_context(context_name, {
            'history': [{
                'date': datetime.now().isoformat(),
                'prompt': prompt,
                'response_summary': summarize_response(response)
            }]
        })
        
    def generate_context_prompt(self, name):
        """Generate a prompt with this context to use with an AI assistant."""
        context = self.get_context(name)
        
        prompt = f"# Context: {context['name']}\n\n"
        prompt += f"{context['description']}\n\n"
        
        if context['key_points']:
            prompt += "## Key Points\n\n"
            for point in context['key_points']:
                prompt += f"- {point}\n"
            prompt += "\n"
            
        if context['decisions']:
            prompt += "## Important Decisions\n\n"
            for decision in context['decisions'][-5:]:  # Last 5 decisions
                date = decision['date'].split('T')[0]
                prompt += f"- [{date}] {decision['decision']}\n"
            prompt += "\n"
            
        if context['files']:
            prompt += "## Relevant Files\n\n"
            for file in context['files']:
                prompt += f"- {file}\n"
            prompt += "\n"
            
        prompt += "Please keep this context in mind for our conversation. I'm continuing our work on this component."
        
        return prompt
        
    def list_contexts(self):
        """List all available contexts."""
        contexts = []
        for filename in os.listdir(self.contexts_dir):
            if filename.endswith('.json'):
                context_path = os.path.join(self.contexts_dir, filename)
                with open(context_path, 'r') as f:
                    context = json.load(f)
                    contexts.append({
                        'name': context['name'],
                        'domain': context['domain'],
                        'updated': context['updated'],
                        'key_points_count': len(context['key_points']),
                        'decisions_count': len(context['decisions'])
                    })
        return contexts
    
    def analyze_codebase(self, extensions=None):
        """Analyze the codebase and suggest context areas."""
        extensions = extensions or ['.js', '.html', '.css', '.json', '.py']
        file_pattern = re.compile(r'|'.join([f'\\{ext}$' for ext in extensions]))
        
        modules = {}
        
        for root, _, files in os.walk(self.project_root):
            # Skip .git, node_modules and other common folders to ignore
            if any(ignore in root for ignore in ['.git', 'node_modules', '__pycache__', 'venv']):
                continue
                
            for file in files:
                if file_pattern.search(file):
                    filepath = os.path.join(root, file)
                    rel_path = os.path.relpath(filepath, self.project_root)
                    
                    # Try to identify module by directory
                    parts = rel_path.split(os.sep)
                    if len(parts) > 1:
                        module = parts[0]
                        if module not in modules:
                            modules[module] = []
                        modules[module].append(rel_path)
        
        suggested_contexts = []
        for module, files in modules.items():
            suggested_contexts.append({
                'name': module,
                'file_count': len(files),
                'sample_files': files[:5]  # First 5 files as samples
            })
            
        return suggested_contexts

def summarize_response(response, max_length=200):
    """Create a short summary of an AI response."""
    if len(response) <= max_length:
        return response
    
    # Simple truncation with ellipsis
    return response[:max_length-3] + '...'

def main():
    parser = argparse.ArgumentParser(description='Manage AI collaboration contexts')
    subparsers = parser.add_subparsers(dest='command', help='Command to run')
    
    # List contexts
    list_parser = subparsers.add_parser('list', help='List all contexts')
    
    # Create context
    create_parser = subparsers.add_parser('create', help='Create a new context')
    create_parser.add_argument('name', help='Name for the new context')
    create_parser.add_argument('--description', help='Description of the context')
    create_parser.add_argument('--domain', help='Domain/category of the context')
    
    # Get context prompt
    prompt_parser = subparsers.add_parser('prompt', help='Generate a prompt for a context')
    prompt_parser.add_argument('name', help='Context name')
    
    # Add decision
    decision_parser = subparsers.add_parser('decision', help='Add a decision to a context')
    decision_parser.add_argument('name', help='Context name')
    decision_parser.add_argument('decision', help='Decision text')
    
    # Analyze codebase
    analyze_parser = subparsers.add_parser('analyze', help='Analyze codebase for context suggestions')
    
    args = parser.parse_args()
    
    manager = AIContextManager()
    
    if args.command == 'list':
        contexts = manager.list_contexts()
        print(f"Found {len(contexts)} contexts:")
        for ctx in contexts:
            print(f"- {ctx['name']} ({ctx['domain']}, {ctx['key_points_count']} key points)")
            
    elif args.command == 'create':
        manager.create_context(args.name, args.description, args.domain)
        
    elif args.command == 'prompt':
        prompt = manager.generate_context_prompt(args.name)
        print("\n" + "="*50 + "\n")
        print(prompt)
        print("\n" + "="*50 + "\n")
        
    elif args.command == 'decision':
        manager.add_decision(args.name, args.decision)
        print(f"Added decision to context '{args.name}'")
        
    elif args.command == 'analyze':
        suggestions = manager.analyze_codebase()
        print(f"Found {len(suggestions)} potential context areas:")
        for suggestion in suggestions:
            print(f"\n- {suggestion['name']} ({suggestion['file_count']} files)")
            print("  Sample files:")
            for sample in suggestion['sample_files']:
                print(f"  - {sample}")
    
    else:
        parser.print_help()

if __name__ == '__main__':
    main()
